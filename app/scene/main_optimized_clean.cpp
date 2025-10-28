#include <flecs.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <format>
#include <vector>
#include "batched_renderer.hpp"

auto main() -> int
{
	// Window configuration
	const int screenWidth = 1024;
	const int screenHeight = 768;

	// Set MSAA 4x anti-aliasing before window creation
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	InitWindow(screenWidth, screenHeight, "OPTIMIZED 3D Scene - Batched Rendering");

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

	// Generate meshes for instanced rendering
	Mesh cubeMesh = GenMeshCube(2.0F, 2.0F, 2.0F);
	Mesh cylinderMesh = GenMeshCylinder(0.5F, 1.0F, 32);
	Mesh sphereMesh = GenMeshSphere(0.8F, 16, 32);
	Mesh lightSphereMesh = GenMeshSphere(0.3F, 8, 32);

	// Note: Meshes are automatically uploaded to GPU by GenMesh functions

	// Create materials for each object type
	Material cubeMaterial = LoadMaterialDefault();
	cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = RED;
	cubeMaterial.maps[MATERIAL_MAP_SPECULAR].color = YELLOW;
	cubeMaterial.shader = lightingShader;

	Material cylinderMaterial = LoadMaterialDefault();
	cylinderMaterial.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
	cylinderMaterial.shader = lightingShader;

	Material sphereMaterial = LoadMaterialDefault();
	sphereMaterial.maps[MATERIAL_MAP_DIFFUSE].color = GREEN;
	sphereMaterial.shader = lightingShader;

	Material lightMaterial = LoadMaterialDefault();
	lightMaterial.maps[MATERIAL_MAP_DIFFUSE].color = YELLOW;

	// Create instanced renderers
	InstancedRenderer cubeRenderer(cubeMesh, cubeMaterial);
	InstancedRenderer cylinderRenderer(cylinderMesh, cylinderMaterial);
	InstancedRenderer sphereRenderer(sphereMesh, sphereMaterial);

	// Light sphere model (not instanced since there's only one)
	Model lightSphereModel = LoadModelFromMesh(lightSphereMesh);
	lightSphereModel.materials[0] = lightMaterial;

	// Generate positions and populate instanced renderers
	const auto generate_and_add_instances = [](InstancedRenderer& renderer, std::size_t count, float radius, float scale)
	{
		// Seed random number generator
		SetRandomSeed(42); // Fixed seed for consistent results

		for (std::size_t i = 0; i < count; ++i)
		{
			// Generate random spherical coordinates
			float theta = GetRandomValue(0, 360) * DEG2RAD; // Random angle 0-360 degrees
			float phi = GetRandomValue(-90, 90) * DEG2RAD;	// Random elevation -90 to 90 degrees

			// Convert spherical to cartesian coordinates
			Vector3 position;
			position.x = radius * cosf(phi) * cosf(theta);
			position.y = radius * sinf(phi);
			position.z = radius * cosf(phi) * sinf(theta);

			// Random rotation
			Vector3 rotation = {GetRandomValue(0, 360) * DEG2RAD, GetRandomValue(0, 360) * DEG2RAD, GetRandomValue(0, 360) * DEG2RAD};

			Vector3 scaleVec = {scale, scale, scale};

			renderer.AddInstance(position, rotation, scaleVec);
		}
	};

	// PERFORMANCE TEST: Start with manageable numbers, then scale up
	printf("=== OPTIMIZED BATCHED RENDERING TEST ===\n");

	// Scale up for performance testing
	generate_and_add_instances(cubeRenderer, 10000, 250.0F, 0.8F);	  // 2000 cubes
	generate_and_add_instances(cylinderRenderer, 5000, 100.0F, 0.8F); // 1500 cylinders
	generate_and_add_instances(sphereRenderer, 2500, 50.0F, 0.8F);	  // 1000 spheres

	printf("Created %zu cube instances\n", cubeRenderer.GetInstanceCount());
	printf("Created %zu cylinder instances\n", cylinderRenderer.GetInstanceCount());
	printf("Created %zu sphere instances\n", sphereRenderer.GetInstanceCount());
	printf("Total objects: %zu\n", cubeRenderer.GetInstanceCount() + cylinderRenderer.GetInstanceCount() + sphereRenderer.GetInstanceCount());

	while (!WindowShouldClose())
	{
		// Update orbit camera
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

		// Draw the light source
		DrawModel(lightSphereModel, lightPosition, 1.0F, WHITE);

		// OPTIMIZED BATCHED RENDERING: 3 draw call groups instead of thousands!
		cubeRenderer.DrawInstanced();
		cylinderRenderer.DrawInstanced();
		sphereRenderer.DrawInstanced();

		EndMode3D();

		// UI with performance info
		auto totalObjects = cubeRenderer.GetInstanceCount() + cylinderRenderer.GetInstanceCount() + sphereRenderer.GetInstanceCount();
		auto formatted = std::format("OPTIMIZED: {} objects in 3 batched draw calls!", totalObjects);

		DrawText(formatted.c_str(), 10, 10, 20, LIME);
		DrawText("- Compare to original: thousands of individual DrawModel calls!", 10, 35, 16, LIGHTGRAY);
		DrawText("- Left click + drag to orbit", 10, 55, 16, LIGHTGRAY);
		DrawText("- Mouse wheel to zoom", 10, 75, 16, LIGHTGRAY);
		DrawText("- MUCH better performance!", 10, 95, 16, LIME);

		DrawFPS(screenWidth - 95, 10);

		EndDrawing();
	}

	// Cleanup resources
	UnloadModel(lightSphereModel);
	UnloadShader(lightingShader);

	CloseWindow();

	return 0;
}