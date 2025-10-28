#include <raylib.h>
#include <raymath.h>
#include "batched_renderer.hpp"

int main()
{
	// Initialize window
	const int screenWidth = 1024;
	const int screenHeight = 768;
	InitWindow(screenWidth, screenHeight, "Dynamic Update Example");
	SetTargetFPS(60);

	// Initialize 3D camera
	Camera3D camera = {0};
	camera.position = {10.0f, 10.0f, 10.0f};
	camera.target = {0.0f, 0.0f, 0.0f};
	camera.up = {0.0f, 1.0f, 0.0f};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	// Light setup
	Vector3 lightPosition = {0.0f, 0.0f, 0.0f};

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

out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragPosition;
out vec3 fragNormal;

void main()
{
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    
    vec4 worldPosition = matModel * vec4(vertexPosition, 1.0);
    fragPosition = worldPosition.xyz;
    fragNormal = normalize(mat3(matNormal) * vertexNormal);
    
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
)";

	const char* fragmentShaderCode = R"(
#version 330 core

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragPosition;
in vec3 fragNormal;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

out vec4 finalColor;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 color = colDiffuse.rgb;
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(lightPos - fragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(viewPos - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 64.0);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * color;
    finalColor = vec4(result, colDiffuse.a);
}
)"; // Load custom shader
	Shader lightingShader = LoadShaderFromMemory(vertexShaderCode, fragmentShaderCode);
	int lightPosLoc = GetShaderLocation(lightingShader, "lightPos");
	int viewPosLoc = GetShaderLocation(lightingShader, "viewPos");
	int lightColorLoc = GetShaderLocation(lightingShader, "lightColor");

	// Create base mesh and material
	Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
	Material material = LoadMaterialDefault();
	material.shader = lightingShader;

	// Set material colors
	material.maps[MATERIAL_MAP_DIFFUSE].color = RED; // Create renderer
	InstancedRenderer renderer(cubeMesh, material);

	// Add some initial cubes
	renderer.AddInstance({-2.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
	renderer.AddInstance({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
	renderer.AddInstance({2.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f});

	float time = 0.0f;

	while (!WindowShouldClose())
	{
		time += GetFrameTime();

		// Update camera
		UpdateCamera(&camera, CAMERA_ORBITAL);

		// Update the middle cube (index 1) to rotate over time
		Vector3 rotation = {0.0f, time * 1.0f, 0.0f}; // Rotate around Y axis

		// Move in a circle around the center point (0,0,0)
		float circleRadius = 2.5f;
		float circleSpeed = 0.5f;
		Vector3 position = {
			cosf(time * circleSpeed) * circleRadius, // X position - circular motion
			0.0f,									 // Y position - keep at ground level
			sinf(time * circleSpeed) * circleRadius	 // Z position - circular motion
		};

		Vector3 scale = {1.0f + sinf(time * 2.0f) * 0.3f, 1.0f, 1.0f}; // Scale along X

		// This will update just this one instance without rebuilding the entire mesh!
		renderer.UpdateInstance(1, position, rotation, scale);

		// Update lighting uniforms
		Vector3 lightColor = {1.0f, 1.0f, 1.0f}; // White light
		SetShaderValue(lightingShader, lightPosLoc, &lightPosition, SHADER_UNIFORM_VEC3);
		SetShaderValue(lightingShader, viewPosLoc, &camera.position, SHADER_UNIFORM_VEC3);
		SetShaderValue(lightingShader, lightColorLoc, &lightColor, SHADER_UNIFORM_VEC3);

		// Begin drawing
		BeginDrawing();
		ClearBackground(DARKBROWN);

		BeginMode3D(camera);

		// Draw grid for reference
		DrawGrid(10, 1.0f);

		// Draw all instances in one call (batched)
		renderer.DrawInstanced();

		EndMode3D();

		// Draw UI
		DrawText("Dynamic Update Example", 10, 10, 20, DARKGRAY);
		DrawText("Middle cube orbits in a circle", 10, 35, 16, GRAY);
		DrawText("Only that cube's vertices are updated on GPU", 10, 55, 16, GRAY);

		EndDrawing();
	}

	// Cleanup - proper order to avoid double-free
	UnloadMesh(cubeMesh);		  // Unload the base mesh first
	UnloadShader(lightingShader); // Then the shader
	// Note: Don't call UnloadMaterial on a material with a custom shader that's already unloaded
	CloseWindow();

	return 0;
}