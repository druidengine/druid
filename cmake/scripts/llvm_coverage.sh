#!/bin/bash

# Generate code coverage report
# Usage: llvm_coverage.sh <SOURCE_ROOT> <BUILD_ROOT>

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <SOURCE_ROOT> <BUILD_ROOT>"
    echo "  SOURCE_ROOT: Path to the project source directory (CMAKE_SOURCE_DIR)"
    echo "  BUILD_ROOT:  Path to the project build directory (CMAKE_BINARY_DIR)"
    exit 1
fi

SOURCE_ROOT="$(realpath "$1")"
BUILD_DIR="$(realpath "$2")"

echo "Source root: $SOURCE_ROOT"
echo "Build directory: $BUILD_DIR"

cd "$BUILD_DIR"

# Merge coverage data
llvm-profdata merge -sparse coverage/*.profraw -o coverage/coverage.profdata

# Find test executables directly in the build directory
# Look for executables with 'test' in the name, excluding dependencies
TEST_EXES=$(find . -name "*test*" -type f -executable -not -path "./_deps/*" -not -name "*.ps1" -not -name "*.sh" | sed "s|^\.|$BUILD_DIR|")

echo "Test executables found:"
for exe in $TEST_EXES; do
    echo "  - $exe"
done

# Find all source files (*.ixx, *.cpp, *.cxx) to include in coverage
cd "$SOURCE_ROOT"
SOURCE_FILES=$(find src app \( -name "*.ixx" -o -name "*.cpp" -o -name "*.cxx" \) -not -path "*/test/*")

echo "Source files for coverage:"
for file in $SOURCE_FILES; do
    echo "  - $file"
done

# Convert to absolute paths
SOURCE_FILES_ABSOLUTE=""
for file in $SOURCE_FILES; do
    SOURCE_FILES_ABSOLUTE="$SOURCE_FILES_ABSOLUTE $SOURCE_ROOT/$file"
done

# Generate separate coverage reports for each test executable
echo "Generating individual coverage reports for each test executable..."

# Create separate directories for each report
mkdir -p "$BUILD_DIR/coverage/html"

# Generate unified report first (for backward compatibility)
echo "Generating unified coverage report..."
TEST_EXES_LIST=$(echo $TEST_EXES | tr '\n' ' ')

llvm-cov show $TEST_EXES_LIST \
    -instr-profile="$BUILD_DIR/coverage/coverage.profdata" \
    -format=html \
    -output-dir="$BUILD_DIR/coverage/html/unified" \
    -show-line-counts-or-regions \
    $SOURCE_FILES_ABSOLUTE

echo "Unified coverage report generated in $BUILD_DIR/coverage/html/unified/"

# Generate individual reports for each test executable
echo ""
echo "Generating individual coverage reports..."

for exe in $TEST_EXES; do
    exe_name=$(basename "$exe")
    echo "Processing $exe_name..."
    
    # Create output directory for this executable
    output_dir="$BUILD_DIR/coverage/html/$exe_name"
    mkdir -p "$output_dir"
    
    # Generate individual coverage report
    llvm-cov show "$exe" \
        -instr-profile="$BUILD_DIR/coverage/coverage.profdata" \
        -format=html \
        -output-dir="$output_dir" \
        -show-line-counts-or-regions \
        $SOURCE_FILES_ABSOLUTE
    
    echo "  → Report generated in $output_dir/"
    
    # Generate summary for this executable
    echo "  → Coverage summary:"
    llvm-cov report "$exe" \
        -instr-profile="$BUILD_DIR/coverage/coverage.profdata" \
        $SOURCE_FILES_ABSOLUTE 2>/dev/null | head -20
    echo ""
done

# Create an index file to navigate between reports
echo "Creating navigation index..."
cat > "$BUILD_DIR/coverage/html/index.html" << 'EOF'
<!DOCTYPE html>
<html>
<head>
    <title>Druid Code Coverage Reports</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        h1 { color: #333; }
        .report-link { 
            display: block; 
            margin: 10px 0; 
            padding: 10px; 
            background-color: #f5f5f5; 
            text-decoration: none; 
            border-radius: 5px;
            color: #333;
        }
        .report-link:hover { background-color: #e5e5e5; }
        .description { color: #666; margin: 5px 0; }
    </style>
</head>
<body>
    <h1>Druid Code Coverage Reports</h1>
    <p>Select a coverage report to view:</p>
    
    <a href="unified/index.html" class="report-link">
        <strong>Unified Coverage Report</strong>
        <div class="description">Combined coverage from all test executables</div>
    </a>
EOF

# Add links for each individual report
for exe in $TEST_EXES; do
    exe_name=$(basename "$exe")
    cat >> "$BUILD_DIR/coverage/html/index.html" << EOF
    
    <a href="$exe_name/index.html" class="report-link">
        <strong>$exe_name Coverage Report</strong>
        <div class="description">Coverage specific to $exe_name</div>
    </a>
EOF
done

cat >> "$BUILD_DIR/coverage/html/index.html" << 'EOF'
    
    <div style="margin-top: 30px; color: #666; font-size: 0.9em;">
        <p><strong>Note:</strong> The unified report may not show all files in the main index due to llvm-cov merging behavior. Individual reports show complete coverage for each test executable.</p>
    </div>
</body>
</html>
EOF

echo "Navigation index created at $BUILD_DIR/coverage/html/index.html"

# Summary
echo ""
echo "=============================="
echo "Coverage Reports Summary:"
echo "=============================="
echo "Main Index: $BUILD_DIR/coverage/html/index.html"
echo "Unified Report: $BUILD_DIR/coverage/html/unified/index.html"
for exe in $TEST_EXES; do
    exe_name=$(basename "$exe")
    echo "Individual Report ($exe_name): $BUILD_DIR/coverage/html/$exe_name/index.html"
done