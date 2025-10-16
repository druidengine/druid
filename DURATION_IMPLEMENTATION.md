# Duration Time Conversion Implementation

## Summary
This implementation resolves the feature request for simplified time conversion utilities using `std::chrono`.

## Changes Made

### 1. Created `Duration.ixx` Module
**Location:** `src/core/Duration.ixx`

**Features:**
- **Type Alias:** `druid::core::seconds` - A `std::chrono::duration<double>` type for fractional seconds
  - Essential for physics and math calculations requiring precise fractional time values
  
- **Conversion Functions:**
  - `to_seconds()` - Converts `std::chrono::steady_clock::duration` to fractional seconds
  - `to_milliseconds()` - Converts to `std::chrono::milliseconds`
  - `to_microseconds()` - Converts to `std::chrono::microseconds`
  - `to_nanoseconds()` - Converts to `std::chrono::nanoseconds`

**Design Benefits:**
- All functions are `constexpr` and `noexcept` for optimal performance
- Fully documented with Doxygen-style comments
- Consistent with existing codebase style (using `[[nodiscard]]`, `auto`, etc.)

### 2. Created `Duration.test.cpp` Test Suite
**Location:** `src/core/test/Duration.test.cpp`

**Test Coverage:**
- ✅ Type alias functionality and fractional value support
- ✅ Conversion from various time units to seconds
- ✅ Conversion between all supported time units
- ✅ Precision tests for fractional seconds
- ✅ Edge cases (zero duration, very small values)
- ✅ Compile-time evaluation (`constexpr` support)
- ✅ Arithmetic operations with the `seconds` type

**Total Tests:** 14 comprehensive test cases

### 3. Updated Build Configuration
- ✅ Added `Duration.ixx` to `src/core/CMakeLists.txt`
- ✅ Added `Duration.test.cpp` to `src/core/test/CMakeLists.txt`

## Usage Examples

```cpp
#include <chrono>
import druid.core.duration;

using namespace std::chrono_literals;
using druid::core::seconds;
using druid::core::to_seconds;

// Fractional seconds for physics calculations
seconds deltaTime = to_seconds(16ms);  // 0.016 seconds (60 FPS)

// Convert steady_clock duration to different units
auto duration = std::chrono::steady_clock::now() - start_time;
auto ms = to_milliseconds(duration);
auto us = to_microseconds(duration);
auto ns = to_nanoseconds(duration);

// Use in calculations
auto velocity = distance / to_seconds(duration).count();
```

## Benefits

1. **Cleaner Code:** Eliminates verbose `std::chrono::duration_cast` calls
2. **Physics-Friendly:** The `seconds` type as `double` is perfect for mathematical calculations
3. **Type Safety:** Maintains strong typing while simplifying conversions
4. **Performance:** All functions are `constexpr` and `noexcept`
5. **Maintainability:** Simple functional approach over class-based solution

## Testing

To build and run the tests:

```bash
# Configure the project
cmake --preset <your-preset>

# Build
cmake --build --preset <your-preset>

# Run tests
ctest --preset <your-preset>
```

The Duration tests will be included in the `druid-core-test` executable.

## Files Modified/Created

- ✅ `src/core/Duration.ixx` (new)
- ✅ `src/core/test/Duration.test.cpp` (new)
- ✅ `src/core/CMakeLists.txt` (modified)
- ✅ `src/core/test/CMakeLists.txt` (modified)

---

**Status:** ✅ Feature Complete - Ready for Review
