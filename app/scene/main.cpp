#include <flecs.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <format>
#include <vector>

auto main() -> int
{
	// Window configuration
	const int screenWidth = 1024;
	const int screenHeight = 768;

	// Set MSAA 4x anti-aliasing before window creation
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	InitWindow(screenWidth, screenHeight, "3D Cube with Orbit Camera");

	// Enable V-Sync to prevent screen tearing
	SetWindowState(FLAG_VSYNC_HINT);

	// Set target FPS to match display refresh rate (usually 60Hz on most Macs)
	SetTargetFPS(120);

	// Camera setup - orbit camera
	Camera3D camera = {};
	camera.position = Vector3{5.0F, 5.0F, 5.0F}; // Camera position
	camera.target = Vector3{0.0F, 0.0F, 0.0F};	 // Camera looking at point
	camera.up = Vector3{0.0F, 1.0F, 0.0F};		 // Camera up vector (rotation towards target)
	camera.fovy = 45.0F;						 // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;		 // Camera mode type

	// Orbit camera parameters
	float orbitRadius = 8.0F;
	float orbitAngle = 0.0F;
	float orbitHeight = 3.0F;

	// Light setup
	Vector3 lightPosition = Vector3{0.0F, 0.0F, 0.0F};
	// float lightAngle = 0.0F;

	// Custom lighting shader code
	const char* vertexShaderCode = R"(
#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec4 vertexColor;

uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matNormal;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec4 fragColor;

void main()
{
    fragPos = vec3(matModel * vec4(vertexPosition, 1.0));
    fragTexCoord = vertexTexCoord;
    fragNormal = normalize(vec3(matNormal * vec4(vertexNormal, 0.0)));
    fragColor = vertexColor;
    
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
)";

	const char* fragmentShaderCode = R"(
#version 330 core

in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

out vec4 finalColor;

void main()
{
    vec3 color = colDiffuse.rgb;
    
    // Ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse lighting
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * color;
    finalColor = vec4(result, colDiffuse.a);
}
)";

	// Load the custom lighting shader
	Shader lightingShader = LoadShaderFromMemory(vertexShaderCode, fragmentShaderCode);

	// Get shader uniform locations
	int lightPosLoc = GetShaderLocation(lightingShader, "lightPos");
	int viewPosLoc = GetShaderLocation(lightingShader, "viewPos");
	int lightColorLoc = GetShaderLocation(lightingShader, "lightColor");

	// Generate meshes (will be uploaded automatically when creating models)
	Mesh cubeMesh = GenMeshCube(2.0F, 2.0F, 2.0F);
	Mesh cylinderMesh = GenMeshCylinder(0.5F, 1.0F, 32);
	Mesh sphereMesh = GenMeshSphere(0.8F, 16, 32);
	Mesh lightSphereMesh = GenMeshSphere(0.3F, 8, 32);

	// Create materials for each object
	Material cubeMaterial = LoadMaterialDefault();
	cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = RED;
	cubeMaterial.maps[MATERIAL_MAP_SPECULAR].color = YELLOW;

	Material cylinderMaterial = LoadMaterialDefault();
	cylinderMaterial.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;

	Material sphereMaterial = LoadMaterialDefault();
	sphereMaterial.maps[MATERIAL_MAP_DIFFUSE].color = GREEN;

	Material lightMaterial = LoadMaterialDefault();
	lightMaterial.maps[MATERIAL_MAP_DIFFUSE].color = YELLOW;

	// Models for easier manipulation
	Model cubeModel = LoadModelFromMesh(cubeMesh);
	cubeModel.materials[0] = cubeMaterial;
	// Apply lighting shader to the cube
	cubeModel.materials[0].shader = lightingShader;

	Model cylinderModel = LoadModelFromMesh(cylinderMesh);
	cylinderModel.materials[0] = cylinderMaterial;
	cylinderModel.materials[0].shader = lightingShader;

	Model sphereModel = LoadModelFromMesh(sphereMesh);
	sphereModel.materials[0] = sphereMaterial;
	sphereModel.materials[0].shader = lightingShader;

	Model lightSphereModel = LoadModelFromMesh(lightSphereMesh);
	lightSphereModel.materials[0] = lightMaterial;

	const auto generate_positions = [](std::size_t count, auto radius)
	{
		// Generate 300 random cube positions in spherical distribution
		std::vector<Vector3> v{count};

		// Seed random number generator
		SetRandomSeed(42); // Fixed seed for consistent results

		for (auto& pos : v)
		{
			// Generate random spherical coordinates
			// float radius = GetRandomValue(50, 200) / 10.0F; // Random radius between 5 and 20
			float theta = GetRandomValue(0, 360) * DEG2RAD; // Random angle 0-360 degrees
			float phi = GetRandomValue(-90, 90) * DEG2RAD;	// Random elevation -90 to 90 degrees

			// Convert spherical to cartesian coordinates
			pos.x = radius * cosf(phi) * cosf(theta);
			pos.y = radius * sinf(phi);
			pos.z = radius * cosf(phi) * sinf(theta);
		}

		return v;
	};

	const auto cubePositions = generate_positions(10000, 250);
	const auto cylinderPositions = generate_positions(5000, 100);
	const auto spherePositions = generate_positions(2500, 50);

	while (!WindowShouldClose())
	{
		// Update orbit camera
		// orbitAngle += 1.0F * GetFrameTime(); // Rotate at 1 radian per second

		// Calculate new camera position
		camera.position.x = cosf(orbitAngle) * orbitRadius;
		camera.position.z = sinf(orbitAngle) * orbitRadius;
		camera.position.y = orbitHeight;

		// Allow manual control with mouse for additional rotation
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Vector2 mouseDelta = GetMouseDelta();
			orbitAngle -= mouseDelta.x * 0.01F;
			orbitHeight += mouseDelta.y * 0.01F;

			// Clamp height
			if (orbitHeight < 1.0F)
				orbitHeight = 1.0F;
			if (orbitHeight > 1000.0F)
				orbitHeight = 1000.0F;
		}

		// Zoom with mouse wheel
		orbitRadius -= GetMouseWheelMove() * 0.5F;
		if (orbitRadius < 2.0F)
			orbitRadius = 2.0F;
		if (orbitRadius > 1000.0F)
			orbitRadius = 1000.0F;

		BeginDrawing();

		ClearBackground(DARKGRAY);

		BeginMode3D(camera);

		// Update shader uniforms for lighting
		SetShaderValue(lightingShader, lightPosLoc, &lightPosition, SHADER_UNIFORM_VEC3);
		SetShaderValue(lightingShader, viewPosLoc, &camera.position, SHADER_UNIFORM_VEC3);
		Vector3 lightColor = Vector3{1.0F, 1.0F, 1.0F}; // White light
		SetShaderValue(lightingShader, lightColorLoc, &lightColor, SHADER_UNIFORM_VEC3);

		// Draw a simple grid
		DrawGrid(10, 1.0F);

		// Draw the light source using mesh
		DrawModel(lightSphereModel, lightPosition, 1.0F, WHITE);

		// Draw 300 cubes at random spherical positions with lighting shader
		for (const auto& pos : cubePositions)
		{
			DrawModel(cubeModel, pos, 0.5F, WHITE); // Smaller scale to avoid overlap
		}

		for (const auto& pos : cylinderPositions)
		{
			DrawModel(cylinderModel, pos, 0.5, WHITE);
		}

		for (const auto& pos : spherePositions)
		{
			DrawModel(sphereModel, pos, 0.5F, WHITE);
		}

		EndMode3D();

		const auto formatted = std::format("3D Scene: {} Cubes {} Cylinders {} Spheres + MSAA + VSYNC", cubePositions.size(),
										   cylinderPositions.size(), spherePositions.size());

		// Draw UI
		DrawText(formatted.c_str(), 10, 10, 20, GRAY);
		DrawText("- Left click + drag to manually control orbit", 10, 35, 16, LIGHTGRAY);
		DrawText("- Mouse wheel to zoom in/out", 10, 55, 16, LIGHTGRAY);
		DrawText("- Yellow sphere = dynamic light source", 10, 95, 16, LIGHTGRAY);
		DrawText("- 4x MSAA anti-aliasing for smooth edges", 10, 135, 16, LIGHTGRAY);

		DrawFPS(screenWidth - 95, 10);

		EndDrawing();
	}

	// Cleanup resources
	UnloadModel(cubeModel);
	UnloadModel(cylinderModel);
	UnloadModel(sphereModel);
	UnloadModel(lightSphereModel);
	UnloadShader(lightingShader);

	CloseWindow();

	return 0;
}
