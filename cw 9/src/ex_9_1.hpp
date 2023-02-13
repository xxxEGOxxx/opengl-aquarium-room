#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <vector>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "SOIL.h"
#include "Texture.h"

#include "Box.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <cmath>
#include <glut.h>


const int DEPTH = 100;
const int NUM_BOIDS = 20;
const float BOID_SIZE = 0.05f;
const float MAX_SPEED = 0.02f;
const float NEIGHBOR_RADIUS = 0.1f;
const float SEPARATION_WEIGHT = 1.0f;
const float ALIGNMENT_WEIGHT = 1.0f;
const float COHESION_WEIGHT = 1.0f;

//const unsigned int SHADOW_WIDTH = 16384, SHADOW_HEIGHT = 16384;
const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;

int WIDTH = 500, HEIGHT = 500;

namespace models {
	Core::RenderContext bedContext;
	Core::RenderContext chairContext;
	Core::RenderContext deskContext;
	Core::RenderContext doorContext;
	Core::RenderContext drawerContext;
	Core::RenderContext marbleBustContext;
	Core::RenderContext materaceContext;
	Core::RenderContext pencilsContext;
	Core::RenderContext floorContext;
	Core::RenderContext roomContext;
	Core::RenderContext spaceshipContext;
	Core::RenderContext sphereContext;
	Core::RenderContext windowContext;
	Core::RenderContext testContext;

	Core::RenderContext sofaBaseContext;
	Core::RenderContext sofaContext;
	Core::RenderContext door1Context;
	Core::RenderContext door2Context;
	Core::RenderContext door3Context;
	Core::RenderContext doorhandleContext;

	Core::RenderContext shelfContext;

	Core::RenderContext fishContext;
	Core::RenderContext fish2Context;
	Core::RenderContext fish3Context;
	Core::RenderContext glassWallContext;

	Core::RenderContext landContext;
	Core::RenderContext roofContext;
	Core::RenderContext door_next_toContext;
	Core::RenderContext door_next_to_doorhandleContext;

	Core::RenderContext aquariumContext;

	Core::RenderContext plant1Context;
	Core::RenderContext plant1DirtContext;
	Core::RenderContext plant1PotContext;

	Core::RenderContext defaultTerrainContext;
	Core::RenderContext algae1Context;
	Core::RenderContext algae2Context;
	Core::RenderContext algae3Context;

	Core::RenderContext glassWindowContext;


	//Procedurally Generated Terrain
	Core::RenderContext ProcedurallyGT;
}

namespace texture {
	GLuint floorTexture;
	GLuint roomTexture;
	GLuint fishTexture;
	GLuint fishRedTexture;
	GLuint fishBlueTexture;
	GLuint fishGreenTexture;
	GLuint sofaTexture;
	GLuint sofaBaseTexture;

	GLuint door1Texture;
	GLuint door2Texture;
	GLuint door3Texture;

	GLuint doorhandleTexture;
	GLuint glassWallTexture;
	GLuint landTexture;
	GLuint roofTexture;
	GLuint door_next_toTexture;
	GLuint door_next_to_doorhandleTexture;
	GLuint aquariumTexture;
	GLuint spaceshipTexture;

	GLuint plant1Texture;
	GLuint plant1DirtTexture;
	GLuint plant1PotTexture;

	GLuint defaultTerrainTexture;
	GLuint algaeTexture;

	GLuint glassWindowTexture;

}

GLuint depthMapFBO;
GLuint depthMap;

GLuint program;
GLuint programSkybox;
GLuint programSun;
GLuint programTest;
GLuint programTex;
GLuint programTexLarge;
GLuint programDepth;

Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;

//glm::vec3 sunPos = glm::vec3(-4.740971f, 1.149999f, 0.369280f);
glm::vec3 sunPos = glm::vec3(-9.5, 4, -5);
glm::vec3 sunDir = glm::vec3(-1.93633f, 0.351106, -1.003226f);
glm::vec3 sunColor = glm::vec3(0.9f, 0.6f, 0.7f) * 1.15;

glm::vec3 cameraPos = glm::vec3(0.1f, 0.1f, 0.1f);
glm::vec3 cameraDir = glm::vec3(0.1f, 0.1f, 0.1f);


glm::vec3 spaceshipPos = glm::vec3(-4.f, 1.25f, -3.5f);
glm::vec3 spaceshipDir = glm::vec3(0.f, 0.f, 1.0f);
GLuint VAO,VBO;

float aspectRatio = 1.f;

float exposition = 1.f;

glm::vec3 lampColor = glm::vec3(0.9f, 0.6f, 0.7f) * 5;
glm::vec3 lampColor2 = glm::vec3(0, 0, 1) * 5;
glm::vec3 lampColor3 = glm::vec3(0, 0, 1) * 5;

glm::vec3 pointlightPos = glm::vec3(-3.5, 2.8, -0.0001);
glm::vec3 pointlightPos2 = glm::vec3(4.5, 2.8, 3);
glm::vec3 pointlightPos3 = glm::vec3(4.5, 2.8, -2.99);

glm::vec3 pointlightColor = glm::vec3(0.9, 0.6, 0.6) * 30;
glm::vec3 pointlightColor2 = glm::vec3(0, 0.2, 1) * 20;
glm::vec3 pointlightColor3 = glm::vec3(0, 0.2, 1) * 20;


glm::vec3 spotlightPos = glm::vec3(0, 0, 0);
glm::vec3 spotlightConeDir = glm::vec3(0, 0, 0);
glm::vec3 spotlightColor = glm::vec3(0.4, 0.4, 0.9)*10;
float spotlightPhi = 3.14 / 4;

float skyboxVertices[] =
{
	-150.0f, -150.0f,  150.0f,
	 150.0f, -150.0f,  150.0f,
	 150.0f, -150.0f, -150.0f,
	-150.0f, -150.0f, -150.0f,
	-150.0f,  150.0f,  150.0f,
	 150.0f,  150.0f,  150.0f,
	 150.0f,  150.0f, -150.0f,
	-150.0f,  150.0f, -150.0f
};

//
unsigned int skyboxIndices[] =
{
	1, 2, 6, 
	6, 5, 1,
	0, 4, 7, 
	7, 3, 0,
	4, 5, 6, 
	6, 7, 4,
	0, 3, 2, 
	2, 1, 0,
	0, 1, 5, 
	5, 4, 0,
	3, 7, 6, 
	6, 2, 3
};

/*
std::string facesCubemap[6] =
{
	"./textures/skybox/rt.png",
	"./textures/skybox/lt.png",
	"./textures/skybox/tp.png",
	"./textures/skybox/bm.png",
	"./textures/skybox/ft.png",
	"./textures/skybox/bk.png"
};
*/
std::string facesCubemap[6] =
{
	"./textures/skybox/space_lf.png",
	"./textures/skybox/space_rt_moon.png",
	"./textures/skybox/space_up.png",
	"./textures/skybox/space_dn.png",
	"./textures/skybox/space_ft.png",
	"./textures/skybox/space_bk.png"
};



std::vector<glm::vec3> vertices;
std::vector<unsigned int> indices;
std::vector<glm::vec3> normals;

// Generate the terrain mesh
void Terraingen() {
	//TODO offset
	const int size = 128;
	const float scaleA = 0.012f;
	const float scaleB = 0.08f;


	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{

			//Generating noise for point based on time
			float noise = glm::simplex(glm::vec2(
				(x - size / 2) * scaleA,
				(y - size / 2) * scaleA
			));
			//<-1,1> ~> <0,1>
			noise = (noise + 1) / 2;
			//x, y, z of a point
			vertices.push_back(glm::vec3(
				((x - size / 2) * scaleB)+5,
				((noise * 1.2f)-0.65f),
				(y - size / 2) * scaleB
			));
		}
	}



	//127 iterations not 128
	for (int x = 0; x < size - 1; x++)
	{
		for (int y = 0; y < size - 1; y++)
		{
			unsigned int i1 = x * size + y;
			unsigned int i2 = (x + 1) * size + y;
			unsigned int i3 = x * size + y + 1;
			unsigned int i4 = (x + 1) * size + y + 1;
			// 1 5 .`
			// 2 .`  5
			// .`  2 6  (for grid 4x4)

			// i1 i2 .`
			// i3  .`  i2
			//   .` i3 i4
			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);

			indices.push_back(i2);
			indices.push_back(i4);
			indices.push_back(i3);
		}
	}

	//Generate normals

	// Get the vertices of each triangle in mesh
	// For each group of indices
	glm::vec3 U, V;
	for (int i = 0; i < indices.size(); i += 3) {

		glm::vec3 a = vertices[indices[i]];
		glm::vec3 b = vertices[indices[i + 1]];
		glm::vec3 c = vertices[indices[i + 2]];

		// Get vectors of two edges of triangle
		U = b - a;
		V = c - a;
		//Append the normalized cross product
		normals.push_back(glm::normalize(-glm::cross(U, V)));
		U = a - b;
		V = c - b;
		normals.push_back(glm::normalize(glm::cross(U, V)));
		U = b - c;
		V = a - c;
		normals.push_back(glm::normalize(glm::cross(U, V)));
		//Total of 3 normals / triangle
	}


}
// code with chatGPT help
//A function to write terrain into a file
void WriteOBJ(const std::string& GeneratedTerrain) {
	// Open the output file
	std::ofstream outFile(GeneratedTerrain);
	if (!outFile.is_open()) {
		// Handle error
		printf("COULDNT OPEN FILE");
		return;
	}

	// Write the vertices
	for (const auto& vertex : vertices) {
		outFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	}

	// Write the normals
	for (const auto& normal : normals) {
		outFile << "vn " << normal.x << " " << normal.y << " " << normal.z << std::endl;
	}

	// Write the indices
	for (int i = 0; i < indices.size(); i += 3) {
		outFile
			<< "f " << indices[i] + 1 << "//" << i + 1 << " "
			<< indices[i + 1] + 1 << "//" << i + 2 << " "
			<< indices[i + 2] + 1 << "//" << i + 3 << std::endl;
	}

	// Close the output file
	outFile.close();
}
//





unsigned int cubemapTexture;

unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
//

bool animal_in_box = true;
bool animal_in_hand = false;
bool animal_in_aquarium = false;

float lastTime = -1.f;
float deltaTime = 0.f;



void updateDeltaTime(float time) {
	if (lastTime < 0) {
		lastTime = time;
		return;
	}

	deltaTime = time - lastTime;
	if (deltaTime > 0.1) deltaTime = 0.1;
	lastTime = time;
}



struct Boid
{
	float x, y, z;
	float vx, vy, vz;
};

std::vector<Boid> boids;

void initBoids()
{
	for (int i = 0; i < NUM_BOIDS; i++)
	{
		Boid b;
		//b.x = float(rand()) / RAND_MAX * 2 - 1;
		b.x = 12;
		b.y = 1;
		b.z = 4;
		b.vx = ((float)rand() / RAND_MAX) * MAX_SPEED * 2.0f - MAX_SPEED;
		b.vy = ((float)rand() / RAND_MAX) * MAX_SPEED * 2.0f - MAX_SPEED;
		b.vz = ((float)rand() / RAND_MAX) * MAX_SPEED * 2.0f - MAX_SPEED;
		boids.push_back(b);
	}
}
void updateBoid(Boid& b) {
	float separationX = 0.0f, separationY = 0.0f, separationZ = 0.0f;
	float alignmentX = 0.0f, alignmentY = 0.0f, alignmentZ = 0.0f;
	float cohesionX = 0.0f, cohesionY = 0.0f, cohesionZ = 0.0f;
	int numNeighbors = 0;
	for (int i = 0; i < NUM_BOIDS; i++) {
		Boid b2 = boids[i];
		float dx = b.x - b2.x;
		float dy = b.y - b2.y;
		float dz = b.z - b2.z;
		float dist = sqrt(dx * dx + dy * dy + dz * dz);
		if (dist > 0.0f && dist < NEIGHBOR_RADIUS) {
			numNeighbors++;
			separationX += dx / dist;
			separationY += dy / dist;
			separationZ += dz / dist;
			alignmentX += b2.vx;
			alignmentY += b2.vy;
			alignmentZ += b2.vz;
			cohesionX += b2.x;
			cohesionY += b2.y;
			cohesionZ += b2.z;
		}
	}
	if (numNeighbors > 0) {
		separationX /= (float)numNeighbors;
		separationY /= (float)numNeighbors;
		separationZ /= (float)numNeighbors;
		alignmentX /= (float)numNeighbors;
		alignmentY /= (float)numNeighbors;
		alignmentZ /= (float)numNeighbors;
		cohesionX /= (float)numNeighbors;
		cohesionY /= (float)numNeighbors;
		cohesionZ /= (float)numNeighbors;
		separationX = separationX * SEPARATION_WEIGHT;
		separationY = separationY * SEPARATION_WEIGHT;
		separationZ = separationZ * SEPARATION_WEIGHT;
		alignmentX = alignmentX * ALIGNMENT_WEIGHT;
		alignmentY = alignmentY * ALIGNMENT_WEIGHT;
		alignmentZ = alignmentZ * ALIGNMENT_WEIGHT;
		cohesionX = (cohesionX / (float)numNeighbors - b.x) * COHESION_WEIGHT;
		cohesionY = (cohesionY / (float)numNeighbors - b.y) * COHESION_WEIGHT;
		cohesionZ = (cohesionZ / (float)numNeighbors - b.z) * COHESION_WEIGHT;

		b.vx += separationX + alignmentX + cohesionX;
		b.vy += separationY + alignmentY + cohesionY;
		b.vz += separationZ + alignmentZ + cohesionZ;

		float speed = sqrt(b.vx * b.vx + b.vy * b.vy + b.vz * b.vz);
		if (speed > MAX_SPEED) {
			b.vx = b.vx / speed * MAX_SPEED;
			b.vy = b.vy / speed * MAX_SPEED;
			b.vz = b.vz / speed * MAX_SPEED;
		}
	}

	b.x += b.vx;
	b.y += b.vy;
	b.z += b.vz;

	if (b.x < -1.0f) {
		b.vx = abs(b.vx);
	}
	if (b.x > 4.0f) {
		b.vx = -abs(b.vx);
	}
	if (b.y < -1.0f) {
		b.vy = abs(b.vy);
	}
	if (b.y > 4.0f) {
		b.vy = -abs(b.vy);
	}
	if (b.z < -1.0f) {
		b.vz = abs(b.vz);
	}
	if (b.z > 5.0f) {
		b.vz = -abs(b.vz);
	}

}


/*
void renderBoids()
{
	float time = glfwGetTime();
	for (int i = 0; i < NUM_BOIDS; i++)
	{

		float x = boids[i].x;
		float y = boids[i].y;
		float z = boids[i].z;

		drawObjectPBRWithTexture(models::fish2Context,
			glm::translate(glm::vec3(3.f, 1.0f, 0.45f))
			* glm::rotate(glm::radians(sin(time / 2) * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::eulerAngleY(time - 12) * glm::translate(glm::vec3(1.2f, 0, 0))
			* glm::scale(glm::vec3(0.2f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)),
			texture::fishBlueTexture,
			0.5f, 0.0f, 0);
	}
}*/


glm::mat4 createCameraMatrix()
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir,glm::vec3(0.f,1.f,0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide,cameraDir));
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-cameraDir.x,-cameraDir.y,-cameraDir.z,0,
		0.,0.,0.,1.,
		});
	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);
	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(-cameraPos);

	return cameraMatrix;
}

glm::mat4 createPerspectiveMatrix()
{
	
	glm::mat4 perspectiveMatrix;
	float n = 0.05;
	float f = 20.;
	float a1 = glm::min(aspectRatio, 1.f);
	float a2 = glm::min(1 / aspectRatio, 1.f);
	perspectiveMatrix = glm::mat4({
		1,0.,0.,0.,
		0.,aspectRatio,0.,0.,
		0.,0.,(f+n) / (n - f),2*f * n / (n - f),
		0.,0.,-1.,0.,
		});

	
	perspectiveMatrix=glm::transpose(perspectiveMatrix);

	return perspectiveMatrix;
}

void drawObjectPBR(Core::RenderContext& context, glm::mat4 modelMatrix, glm::vec3 color, float roughness, float metallic) {

	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	glUniform1f(glGetUniformLocation(program, "exposition"), exposition);

	glUniform1f(glGetUniformLocation(program, "roughness"), roughness);
	glUniform1f(glGetUniformLocation(program, "metallic"), metallic);

	glUniform3f(glGetUniformLocation(program, "color"), color.x, color.y, color.z);

	glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	glUniform3f(glGetUniformLocation(program, "sunDir"), sunDir.x, sunDir.y, sunDir.z);
	glUniform3f(glGetUniformLocation(program, "sunColor"), sunColor.x, sunColor.y, sunColor.z);

	glUniform3f(glGetUniformLocation(program, "lightPos"), pointlightPos.x, pointlightPos.y, pointlightPos.z);
	glUniform3f(glGetUniformLocation(program, "lightPos2"), pointlightPos2.x, pointlightPos2.y, pointlightPos2.z);
	glUniform3f(glGetUniformLocation(program, "lightPos3"), pointlightPos3.x, pointlightPos3.y, pointlightPos3.z);
	glUniform3f(glGetUniformLocation(program, "lightColor"), pointlightColor.x, pointlightColor.y, pointlightColor.z);
	glUniform3f(glGetUniformLocation(program, "lightColor2"), pointlightColor2.x, pointlightColor2.y, pointlightColor2.z);
	glUniform3f(glGetUniformLocation(program, "lightColor3"), pointlightColor3.x, pointlightColor3.y, pointlightColor3.z);

	glUniform3f(glGetUniformLocation(program, "spotlightConeDir"), spotlightConeDir.x, spotlightConeDir.y, spotlightConeDir.z);
	glUniform3f(glGetUniformLocation(program, "spotlightPos"), spotlightPos.x, spotlightPos.y, spotlightPos.z);
	glUniform3f(glGetUniformLocation(program, "spotlightColor"), spotlightColor.x, spotlightColor.y, spotlightColor.z);
	glUniform1f(glGetUniformLocation(program, "spotlightPhi"), spotlightPhi);

	// code with chatGPT help
	glm::mat4 lightVP = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.0f, 40.0f) * glm::lookAt(sunPos, sunPos - sunDir, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(program, "LightVP"), 1, GL_FALSE, (float*)&lightVP);
	glUniform1i(glGetUniformLocation(program, "depthMap"), 2);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	Core::DrawContext(context);

}

void drawObjectPBRWithTexture(Core::RenderContext& context, glm::mat4 modelMatrix, GLuint textureID, float roughness, float metallic, float size) {
	glUseProgram(programTex);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;


	glUniform1f(glGetUniformLocation(programTex, "size"), size);


	glUniformMatrix4fv(glGetUniformLocation(programTex, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(programTex, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);


	glUniform1f(glGetUniformLocation(programTex, "exposition"), exposition);

	glUniform1f(glGetUniformLocation(programTex, "roughness"), roughness);
	glUniform1f(glGetUniformLocation(programTex, "metallic"), metallic);


	//glUniform3f(glGetUniformLocation(program, "color"), color.x, color.y, color.z);

	glUniform3f(glGetUniformLocation(programTex, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	glUniform3f(glGetUniformLocation(programTex, "sunDir"), sunDir.x, sunDir.y, sunDir.z);
	glUniform3f(glGetUniformLocation(programTex, "sunColor"), sunColor.x, sunColor.y, sunColor.z);

	glUniform3f(glGetUniformLocation(programTex, "lightPos"), pointlightPos.x, pointlightPos.y, pointlightPos.z);
	glUniform3f(glGetUniformLocation(programTex, "lightPos2"), pointlightPos2.x, pointlightPos2.y, pointlightPos2.z);
	glUniform3f(glGetUniformLocation(programTex, "lightPos3"), pointlightPos3.x, pointlightPos3.y, pointlightPos3.z);
	glUniform3f(glGetUniformLocation(programTex, "lightColor"), pointlightColor.x, pointlightColor.y, pointlightColor.z);
	glUniform3f(glGetUniformLocation(programTex, "lightColor2"), pointlightColor2.x, pointlightColor2.y, pointlightColor2.z);
	glUniform3f(glGetUniformLocation(programTex, "lightColor3"), pointlightColor3.x, pointlightColor3.y, pointlightColor3.z);

	glUniform3f(glGetUniformLocation(programTex, "spotlightConeDir"), spotlightConeDir.x, spotlightConeDir.y, spotlightConeDir.z);
	glUniform3f(glGetUniformLocation(programTex, "spotlightPos"), spotlightPos.x, spotlightPos.y, spotlightPos.z);
	glUniform3f(glGetUniformLocation(programTex, "spotlightColor"), spotlightColor.x, spotlightColor.y, spotlightColor.z);
	glUniform1f(glGetUniformLocation(programTex, "spotlightPhi"), spotlightPhi);

	// code with chatGPT help
	glm::mat4 lightVP = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.0f, 40.0f) * glm::lookAt(sunPos, sunPos - sunDir, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(programTex, "LightVP"), 1, GL_FALSE, (float*)&lightVP);
	glUniform1i(glGetUniformLocation(programTex, "depthMap"), 2);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	Core::SetActiveTexture(textureID, "colorTexture", programTex, 0);
	Core::DrawContext(context);
	glUseProgram(0);
}


void drawObjectDepth(Core::RenderContext context, glm::mat4 viewProjectionMatrix, glm::mat4 modelMatrix) {

	glUniformMatrix4fv(glGetUniformLocation(programDepth, "viewProjectionMatrix"), 1, GL_FALSE, (float*)&viewProjectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(programDepth, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	Core::DrawContext(context);
}

void renderShadowapSun(GLuint depthMapFBO, glm::mat4 lightVP) {
	float time = glfwGetTime();
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//uzupelnij o renderowanie glebokosci do tekstury
	glUseProgram(programDepth);

	//ustawianie przestrzeni rysowania 
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//bindowanie FBO
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//czyszczenie mapy głębokości 
	glClear(GL_DEPTH_BUFFER_BIT);
	//ustawianie programu
	glUseProgram(programDepth);

	drawObjectDepth(models::sofaBaseContext, lightVP, glm::mat4());
	drawObjectDepth(models::sofaContext, lightVP, glm::mat4());
	//drawObjectDepth(models::fishContext, lightVP, glm::mat4());
	drawObjectDepth(models::fish2Context, lightVP, glm::mat4());
	//drawObjectDepth(models::fish3Context, lightVP, glm::mat4());
	drawObjectDepth(models::shelfContext, lightVP, glm::mat4());
	drawObjectDepth(models::pencilsContext, lightVP, glm::mat4());
	drawObjectDepth(models::aquariumContext, lightVP, glm::mat4());
	drawObjectDepth(models::roofContext, lightVP, glm::mat4());
	drawObjectDepth(models::landContext, lightVP, glm::mat4());
	drawObjectDepth(models::floorContext, lightVP, glm::mat4());
	drawObjectDepth(models::marbleBustContext, lightVP, glm::mat4());
	drawObjectDepth(models::roomContext, lightVP, glm::mat4());

	drawObjectDepth(models::door1Context, lightVP, glm::mat4());
	drawObjectDepth(models::door2Context, lightVP, glm::mat4());
	drawObjectDepth(models::door3Context, lightVP, glm::mat4());
	drawObjectDepth(models::doorContext, lightVP, glm::mat4());
	drawObjectDepth(models::doorhandleContext, lightVP, glm::mat4());
	drawObjectDepth(models::door_next_toContext, lightVP, glm::mat4());
	drawObjectDepth(models::door_next_to_doorhandleContext, lightVP, glm::mat4());
	drawObjectDepth(models::defaultTerrainContext, lightVP, glm::mat4());
	drawObjectDepth(models::ProcedurallyGT, lightVP, glm::mat4());

	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::normalize(glm::cross(spaceshipSide, spaceshipDir));
	glm::mat4 specshipCameraRotrationMatrix = glm::mat4({
	  spaceshipSide.x,spaceshipSide.y,spaceshipSide.z,0,
	  spaceshipUp.x,spaceshipUp.y,spaceshipUp.z ,0,
	  -spaceshipDir.x,-spaceshipDir.y,-spaceshipDir.z,0,
	  0.,0.,0.,1.,
		});

	drawObjectDepth(shipContext, lightVP,
		glm::translate(spaceshipPos) * specshipCameraRotrationMatrix * glm::eulerAngleY(glm::pi<float>()) * glm::scale(glm::vec3(0.03f)));
	
	if (animal_in_aquarium) drawObjectDepth(models::fish2Context, lightVP, glm::translate(glm::vec3(3.f, 1.25f, 0.0f))
		* glm::rotate(glm::radians(sin(time) * 10.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::eulerAngleY(time) * glm::translate(glm::vec3(1.5f, 0, 0))
		* glm::eulerAngleY(time) * glm::translate(glm::vec3(1.f, sin(time / 3), 0)) * glm::translate(glm::vec3(0, 0, 0))
		* glm::scale(glm::vec3(0.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))
		);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	drawObjectDepth(models::fish2Context, lightVP, glm::translate(glm::vec3(3.f, 1.0f, 0.45f))
		* glm::rotate(glm::radians(sin(time / 2) * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::eulerAngleY(time - 12) * glm::translate(glm::vec3(1.2f, 0, 0))
		* glm::scale(glm::vec3(0.2f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))
	);
	drawObjectDepth(models::fish2Context, lightVP, glm::translate(glm::vec3(2.75f, 1.3f, -0.2f))
		* glm::rotate(glm::radians(sin(time - 32) * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::eulerAngleY(time) * glm::translate(glm::vec3(1.5f, 0, 0))
		* glm::eulerAngleY(time + 41) * glm::translate(glm::vec3(0.3f, sin(time / 3), 0)) * glm::translate(glm::vec3(0, 0, 0))
		* glm::scale(glm::vec3(0.2f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))
	);
	drawObjectDepth(models::fish2Context, lightVP, glm::translate(glm::vec3(3.f, 1.25f, -0.42f))
		* glm::rotate(glm::radians(sin(time) * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::eulerAngleY(time - 47) * glm::translate(glm::vec3(0.5f, 0, 0))
		* glm::eulerAngleY(time + 25) * glm::translate(glm::vec3(0.25f, sin(time / 3), 0)) * glm::translate(glm::vec3(0, 0, 0))
		* glm::scale(glm::vec3(0.2f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))
	);
	drawObjectDepth(models::fish2Context, lightVP, glm::translate(glm::vec3(2.78f, 0.7f, -0.3f))
		* glm::rotate(glm::radians(sin(time / 2 + 23) * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::eulerAngleY(time - 11) * glm::translate(glm::vec3(1.2f, 0, 0))
		* glm::scale(glm::vec3(0.2f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))
		);

	for (int i = 0; i < NUM_BOIDS; i++)
	{

		float x = boids[i].x;
		float y = boids[i].y;
		float z = boids[i].z;

		drawObjectDepth(models::fish2Context, lightVP,
			glm::translate(glm::vec3(x, y, z))
		//*glm::rotate(glm::radians(sin(time / 2 + 23) * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f))
			//* glm::eulerAngleY(time - 11)* glm::translate(glm::vec3(1.2f, 0, 0))
			//* glm::scale(glm::vec3(0.2f))* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))
			);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glViewport(0, 0, WIDTH, HEIGHT);
}

void initDepthMap()
{
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderScene(GLFWwindow* window)
{
	glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float time = glfwGetTime();
	updateDeltaTime(time);

	//drawing skybox
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glUseProgram(programSkybox);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformationSky = viewProjectionMatrix * glm::translate(cameraPos) * glm::scale(glm::vec3(0.1));
	glUniformMatrix4fv(glGetUniformLocation(programSkybox, "transformation"), 1, GL_FALSE, (float*)&transformationSky);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//
	// code with chatGPT help
	glm::mat4 lightVP = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.0f, 40.0f) * glm::lookAt(sunPos, sunPos - sunDir, glm::vec3(0, 1, 0));
	//
	renderShadowapSun(depthMapFBO, lightVP);

	//space lamp
	glUseProgram(programSun);
	//glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * glm::translate(pointlightPos) * glm::scale(glm::vec3(0.1));
	glUniformMatrix4fv(glGetUniformLocation(programSun, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniform3f(glGetUniformLocation(programSun, "color"), lampColor.x / 2, lampColor.y / 2, lampColor.z / 2);
	glUniform1f(glGetUniformLocation(programSun, "exposition"), exposition);
	Core::DrawContext(sphereContext);

	glm::mat4 transformation2 = viewProjectionMatrix * glm::translate(pointlightPos2) * glm::scale(glm::vec3(0.1));
	glUniformMatrix4fv(glGetUniformLocation(programSun, "transformation"), 1, GL_FALSE, (float*)&transformation2);
	glUniform3f(glGetUniformLocation(programSun, "color"), lampColor2.x / 2, lampColor2.y / 2, lampColor2.z / 2);
	glUniform1f(glGetUniformLocation(programSun, "exposition"), exposition);
	Core::DrawContext(sphereContext);

	glm::mat4 transformation3 = viewProjectionMatrix * glm::translate(pointlightPos3) * glm::scale(glm::vec3(0.1));
	glUniformMatrix4fv(glGetUniformLocation(programSun, "transformation"), 1, GL_FALSE, (float*)&transformation3);
	glUniform3f(glGetUniformLocation(programSun, "color"), lampColor3.x / 2, lampColor3.y / 2, lampColor3.z / 2);
	glUniform1f(glGetUniformLocation(programSun, "exposition"), exposition);
	Core::DrawContext(sphereContext);


	glUseProgram(program);

	/*
	drawObjectPBR(sphereContext,
		glm::translate(pointlightPos) * glm::scale(glm::vec3(0.1)) * glm::eulerAngleY(time / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::scale(glm::vec3(0.3f)),
		glm::vec4(0.2, 0.7, 0.3, 0), 0.3, 0.0);

	drawObjectPBR(sphereContext,
		glm::translate(pointlightPos) * glm::scale(glm::vec3(0.1)) * glm::eulerAngleY(time / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::eulerAngleY(time) * glm::translate(glm::vec3(1.f, 0, 0)) * glm::scale(glm::vec3(0.1f)),
		glm::vec4(0.5, 0.5, 0.5, 0), 0.7, 0.0);
	*/

	//drawObjectPBR(models::bedContext, glm::mat4(), glm::vec3(0.03f, 0.03f, 0.03f), 0.2f, 0.0f);
	//drawObjectPBR(models::chairContext, glm::mat4(), glm::vec3(0.195239f, 0.37728f, 0.8f), 0.4f, 0.0f);
	drawObjectPBR(models::deskContext, glm::mat4(), glm::vec3(0.428691f, 0.08022f, 0.036889f), 0.2f, 0.0f);
	//drawObjectPBR(models::doorContext, glm::mat4(), glm::vec3(0.402978f, 0.120509f, 0.057729f), 0.2f, 0.0f);
	drawObjectPBR(models::drawerContext, glm::mat4(), glm::vec3(0.428691f, 0.08022f, 0.036889f), 0.2f, 0.0f);
	drawObjectPBR(models::marbleBustContext, glm::mat4(), glm::vec3(1.f, 1.f, 1.f), 0.5f, 1.0f);
	//drawObjectPBR(models::materaceContext, glm::mat4(), glm::vec3(0.9f, 0.9f, 0.9f), 0.8f, 0.0f);
	drawObjectPBR(models::pencilsContext, glm::mat4(), glm::vec3(0.10039f, 0.018356f, 0.001935f), 0.1f, 0.0f);


	//drawObjectPBR(models::floorContext, glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f), 0.2f, 0.0f);
	
	//drawObjectPBR(models::roomContext, glm::mat4(), glm::vec3(0.8f, 0.8f, 0.8f), 0.8f, 0.0f);
	//drawObjectPBR(models::windowContext, glm::mat4(), glm::vec3(0.402978f, 0.120509f, 0.057729f), 0.2f, 0.0f);

	//drawObjectPBR(models::sofaContext, glm::mat4(), glm::vec3(0.4f, 0.4f, 0.4f), 0.5f, 1.0f);
	//drawObjectPBR(models::sofaBaseContext, glm::mat4(), glm::vec3(0.3f, 0.3f, 0.3f), 0.5f, 0.0f);

	//drawObjectPBR(models::door1Context, glm::mat4(), glm::vec3(0.4f, 0.4f, 0.4f), 0.2f, 0.0f);
	//drawObjectPBR(models::door2Context, glm::mat4(), glm::vec3(0.4f, 0.4f, 0.4f), 0.2f, 0.0f);
	//drawObjectPBR(models::door3Context, glm::mat4(), glm::vec3(0.402978f, 0.120509f, 0.057729f), 0.2f, 0.0f);

	drawObjectPBR(models::shelfContext, glm::mat4(), glm::vec3(0.2f, 0.2f, 0.2f), 0.5f, 0.0f);
	drawObjectPBR(models::ProcedurallyGT, glm::mat4(), glm::vec3(1.0f, 1.0f, 0.6f), 0.5f, 0.0f);
	//drawObjectPBR(models::landContext, glm::mat4(), glm::vec3(0.5f, 0.5f, 0.1f), 0.5f, 0.0f);

	//drawObjectPBR(models::fishContext, glm::mat4(), glm::vec3(0.7f, 0.2f, 0.1f), 0.5f, 0.0f);
	//drawObjectPBR(models::glassWallContext, glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	drawObjectPBRWithTexture(models::fish2Context,
		glm::translate(glm::vec3(3.f, 1.0f, 0.45f))
		* glm::rotate(glm::radians(sin(time/2) * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::eulerAngleY(time - 12)* glm::translate(glm::vec3(1.2f, 0, 0))
		* glm::scale(glm::vec3(0.2f))* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)),
		texture::fishBlueTexture,
		0.5f, 0.0f, 0);
	
	drawObjectPBRWithTexture(models::fish2Context,
		glm::translate(glm::vec3(2.75f, 1.3f, -0.2f))
		* glm::rotate(glm::radians(sin(time -32) * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::eulerAngleY(time)* glm::translate(glm::vec3(1.5f, 0, 0))
		* glm::eulerAngleY(time+41)* glm::translate(glm::vec3(0.3f, sin(time / 3), 0)) * glm::translate(glm::vec3(0, 0, 0))
		* glm::scale(glm::vec3(0.2f))* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)),
		texture::fishBlueTexture,
		0.5f, 0.0f, 0);
	
	drawObjectPBRWithTexture(models::fish2Context,
		glm::translate(glm::vec3(3.f, 1.25f, -0.42f))
		* glm::rotate(glm::radians(sin(time) * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::eulerAngleY(time - 47)* glm::translate(glm::vec3(0.5f, 0, 0))
		* glm::eulerAngleY(time + 25)* glm::translate(glm::vec3(0.25f, sin(time / 3), 0))* glm::translate(glm::vec3(0, 0, 0))
		* glm::scale(glm::vec3(0.2f))* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)),
		texture::fishGreenTexture,
		0.5f, 0.0f, 0);

	drawObjectPBRWithTexture(models::fish2Context,
		glm::translate(glm::vec3(2.78f, 0.7f, -0.3f))
		* glm::rotate(glm::radians(sin(time / 2 + 23) * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::eulerAngleY(time - 11)* glm::translate(glm::vec3(1.2f, 0, 0))
		* glm::scale(glm::vec3(0.2f))* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)),
		texture::fishGreenTexture,
		0.5f, 0.0f, 0);
		
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::normalize(glm::cross(spaceshipSide, spaceshipDir));
	glm::mat4 specshipCameraRotrationMatrix = glm::mat4({
		spaceshipSide.x,spaceshipSide.y,spaceshipSide.z,0,
		spaceshipUp.x,spaceshipUp.y,spaceshipUp.z ,0,
		-spaceshipDir.x,-spaceshipDir.y,-spaceshipDir.z,0,
		0.,0.,0.,1.,
		});

	if (animal_in_hand) {
		drawObjectPBRWithTexture(models::fish2Context,
			 glm::translate(spaceshipPos) * glm::translate(glm::vec3(0, 0.06f, 0)) * specshipCameraRotrationMatrix * glm::eulerAngleY(glm::pi<float>()) * glm::scale(glm::vec3(0.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, 1)),
			texture::fishRedTexture,
			0.5f, 0.0f, 0);
	}

	if (animal_in_aquarium) {
		drawObjectPBRWithTexture(models::fish2Context,
			glm::translate(glm::vec3(3.f, 1.25f, 0.0f))
			* glm::rotate(glm::radians(sin(time) * 10.0f), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::eulerAngleY(time) * glm::translate(glm::vec3(1.5f, 0, 0))
			* glm::eulerAngleY(time) * glm::translate(glm::vec3(1.f, sin(time / 3), 0)) * glm::translate(glm::vec3(0, 0, 0))
			* glm::scale(glm::vec3(0.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)),
			texture::fishRedTexture,
			0.5f, 0.0f, 0);
	}

	if (animal_in_box) {
		drawObjectPBRWithTexture(models::fish2Context,
			glm::translate(glm::vec3(-6.55f, 1.22f, 0.61f)) 
			* glm::rotate(glm::radians(sin(time) * 15.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::radians(sin(-time) * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f)) 
			* glm::translate(glm::vec3(0.0f, 0.05f * sin(1.0f * time), 0.0f)) * glm::scale(glm::vec3(0.5f)),
			texture::fishRedTexture,
			0.5f, 0.0f, 0);
	}

	spotlightPos = spaceshipPos + 0.2 * spaceshipDir;
	spotlightConeDir = spaceshipDir;

	//draw texture with PBR

	drawObjectPBRWithTexture(shipContext,
		glm::translate(spaceshipPos) * specshipCameraRotrationMatrix * glm::eulerAngleY(glm::pi<float>()) * glm::scale(glm::vec3(0.03f)),
		texture::spaceshipTexture,
		0.2, 1.0, 0
	);

	drawObjectPBRWithTexture(models::floorContext, glm::mat4(), texture::floorTexture, 0.8f, 0.0f,5);
	drawObjectPBRWithTexture(models::roomContext, glm::mat4(), texture::roomTexture, 0.8f, 0.0f, 7);
	//drawObjectPBRWithTexture(models::fishContext, glm::mat4(), texture::fishTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::landContext, glm::mat4(), texture::landTexture, 0.5f, 0.0f, 10);
	drawObjectPBRWithTexture(models::sofaBaseContext, glm::mat4(), texture::sofaBaseTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::sofaContext, glm::mat4(), texture::sofaTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::roofContext, glm::mat4(), texture::roofTexture, 0.5f, 0.0f, 5);
	//drawObjectPBRWithTexture(models::fish2Context, glm::mat4(), texture::fishRedTexture, 0.5f, 0.0f, 0);
	//drawObjectPBRWithTexture(models::fish3Context, glm::mat4(), texture::fishTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door1Context, glm::mat4(), texture::door1Texture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door2Context, glm::mat4(), texture::door2Texture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door3Context, glm::mat4(), texture::door3Texture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::doorhandleContext, glm::mat4(), texture::doorhandleTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door_next_toContext, glm::mat4(), texture::door_next_toTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door_next_to_doorhandleContext, glm::mat4(), texture::door_next_to_doorhandleTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::spaceshipContext, glm::mat4(), texture::spaceshipTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::plant1DirtContext, glm::mat4(), texture::plant1DirtTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::plant1PotContext, glm::mat4(), texture::plant1PotTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::plant1Context, glm::mat4(), texture::plant1Texture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::defaultTerrainContext, glm::mat4(), texture::defaultTerrainTexture, 0.5f, 0.0f, 5);
	drawObjectPBRWithTexture(models::algae1Context, glm::mat4(), texture::algaeTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::algae2Context, glm::mat4(), texture::algaeTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::algae3Context, glm::mat4(), texture::algaeTexture, 0.5f, 0.0f, 0);


	//objects with textures that contain transparency should be drawn here (last)
	drawObjectPBRWithTexture(models::glassWallContext, glm::mat4(), texture::glassWallTexture, 0.8f, 0.0f, 5);
	drawObjectPBRWithTexture(models::aquariumContext, glm::mat4(), texture::aquariumTexture, 0.8f, 0.0f, 5);
	drawObjectPBRWithTexture(models::glassWindowContext, glm::mat4(), texture::glassWallTexture, 0.5f, 0.0f, 5);

	for (int i = 0; i < NUM_BOIDS; i++)
	{

		float x = boids[i].x;
		float y = boids[i].y;
		float z = boids[i].z;

		drawObjectPBRWithTexture(models::fish2Context,
			glm::translate(glm::vec3(x, y, z)),
			//* glm::rotate(glm::radians(sin(time / 2) * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f))
			//* glm::eulerAngleY(time - 12) * glm::translate(glm::vec3(1.2f, 0, 0))
			//* glm::scale(glm::vec3(0.2f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)),
			texture::fishGreenTexture,
			0.5f, 0.0f, 0);
	}
	for (int i = 0; i < NUM_BOIDS; i++) {
		updateBoid(boids[i]);
	}
	//Terraingen();
	



	//test depth buffer
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(programTest);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	Core::DrawContext(models::testContext);*/

	glUseProgram(0);
	glfwSwapBuffers(window);
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / float(height);
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
}
void loadModelToContext(std::string path, Core::RenderContext& context)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	context.initFromAssimpMesh(scene->mMeshes[0]);
}

void init(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	initDepthMap();

	Terraingen();
	std::string GeneratedTerrain = "models/GeneratedTerrain.obj";
	WriteOBJ(GeneratedTerrain);

	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_9_1.vert", "shaders/shader_9_1.frag");
	programTest = shaderLoader.CreateProgram("shaders/test.vert", "shaders/test.frag");
	programSun = shaderLoader.CreateProgram("shaders/shader_8_sun.vert", "shaders/shader_8_sun.frag");
	programDepth = shaderLoader.CreateProgram("shaders/shader_shadow_global_sun.vert", "shaders/shader_shadow_global_sun.frag");

	//loading models 
	loadModelToContext("./models/sphere.obj", sphereContext);
	//model created manually
	loadModelToContext("./models/fighter.obj", shipContext);
	loadModelToContext("./models/Marble_Bust.obj", models::marbleBustContext);
	loadModelToContext("./models/Pencils_Cylinder.obj", models::pencilsContext);
	//model created manually
	loadModelToContext("./models/plane.obj", models::floorContext);
	//model created manually
	loadModelToContext("./models/room.obj", models::roomContext);
	//loadModelToContext("./models/spaceship.obj", models::spaceshipContext);
	//model created manually
	loadModelToContext("./models/sphere.obj", models::sphereContext);
	//model created manually
	loadModelToContext("./models/test.obj", models::testContext);

	//The model is created by modifying the another model
	loadModelToContext("./models/Sofa_baseCube.obj", models::sofaContext);
	//The model is created by modifying the another model
	loadModelToContext("./models/Sofa_base.obj", models::sofaBaseContext);
	loadModelToContext("./models/Door1.obj", models::door1Context);
	loadModelToContext("./models/Door2.obj", models::door2Context);
	loadModelToContext("./models/Door3.obj", models::door3Context);
	loadModelToContext("./models/DoorCircle.obj", models::doorhandleContext);

	//The model is created by modifying the another model
	loadModelToContext("./models/Shelf.obj", models::shelfContext);
	loadModelToContext("./models/fish.obj", models::fishContext);
	loadModelToContext("./models/fish2.obj", models::fish2Context);
	loadModelToContext("./models/fish3.obj", models::fish3Context);
	//model created manually
	loadModelToContext("./models/Glass_wall.obj", models::glassWallContext);
	//model created manually
	loadModelToContext("./models/land.obj", models::landContext);
	//model created manually
	loadModelToContext("./models/roof.obj", models::roofContext);

	loadModelToContext("./models/door_next_to.obj", models::door_next_toContext);
	loadModelToContext("./models/door_next_to_doorhandle.obj", models::door_next_to_doorhandleContext);

	//model created manually
	loadModelToContext("./models/aquarium.obj", models::aquariumContext);

	//The model is created by modifying the another model
	loadModelToContext("./models/plant1.obj", models::plant1Context);
	//The model is created by modifying the another model
	loadModelToContext("./models/plant1dirt.obj", models::plant1DirtContext);
	//model created manually
	loadModelToContext("./models/plant1pot.obj", models::plant1PotContext);

	//model created manually
	loadModelToContext("./models/default_terrain.obj", models::defaultTerrainContext);
	//The model is created by modifying the another model
	loadModelToContext("./models/algae1.obj", models::algae1Context);
	//The model is created by modifying the another model
	loadModelToContext("./models/algae2.obj", models::algae2Context);
	//The model is created by modifying the another model
	loadModelToContext("./models/algae3.obj", models::algae3Context);
	//model created manually
	loadModelToContext("./models/glassWindow.obj", models::glassWindowContext);

	//model created manually(by generating)
	loadModelToContext("./models/GeneratedTerrain.obj", models::ProcedurallyGT);


	//loading textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//textures that contain transparency should be loaded here
	texture::glassWallTexture = Core::LoadTexture("textures/glass.png");
	texture::aquariumTexture = Core::LoadTexture("textures/glass_dark.png");
	glDisable(GL_BLEND);
	programTex = shaderLoader.CreateProgram("shaders/shader_texture.vert", "shaders/shader_texture.frag");
	//texture::glassWallTexture = Core::LoadTexture("textures/glass.jpg");
	texture::fishTexture = Core::LoadTexture("textures/fish.png");
	texture::fishRedTexture = Core::LoadTexture("textures/fish_red.png");
	texture::fishBlueTexture = Core::LoadTexture("textures/fish_blue.png");
	texture::fishGreenTexture = Core::LoadTexture("textures/fish_green.png");
	texture::roomTexture = Core::LoadTexture("textures/wall.jpg");
	texture::sofaBaseTexture = Core::LoadTexture("textures/sofaa.jpg");
	texture::sofaTexture = Core::LoadTexture("textures/sofaa.jpg");
	texture::landTexture = Core::LoadTexture("textures/grass.jpg");
	texture::floorTexture = Core::LoadTexture("textures/floor3.jpg");
	texture::roofTexture = Core::LoadTexture("textures/roof.jpg");
	texture::door1Texture = Core::LoadTexture("textures/Door.jpg");
	texture::door2Texture = Core::LoadTexture("textures/Door.jpg");
	texture::door3Texture = Core::LoadTexture("textures/Door.jpg");
	texture::doorhandleTexture = Core::LoadTexture("textures/Door.jpg");
	texture::door_next_toTexture = Core::LoadTexture("textures/Door.jpg");
	texture::door_next_to_doorhandleTexture = Core::LoadTexture("textures/Door.jpg");
	texture::spaceshipTexture = Core::LoadTexture("textures/spaceship.png");
	texture::plant1Texture = Core::LoadTexture("textures/plant1.jpg");
	texture::plant1DirtTexture = Core::LoadTexture("textures/plant1dirt.jpg");
	texture::plant1PotTexture = Core::LoadTexture("textures/plant1pot.jpg");
	texture::defaultTerrainTexture = Core::LoadTexture("textures/plant1dirt.jpg");
	texture::algaeTexture = Core::LoadTexture("textures/algae.jpg");
	//
	
	//prepering skybox
	programSkybox = shaderLoader.CreateProgram("shaders/shader_skybox.vert", "shaders/shader_skybox.frag");
	glUseProgram(programSkybox);
	glUniform1i(glGetUniformLocation(programSkybox, "skybox"), 0);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < 6; i++)
	{
		int w, h;
		unsigned char* data = SOIL_load_image(facesCubemap[i].c_str(), &w, &h, 0, SOIL_LOAD_RGBA);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
		);
	}
	//


		/*
	glGenVertexArrays(1, &TerrainVAO);
	glGenBuffers(1, &TerrainVBO);
	glGenBuffers(1, &TerrainIBO);
	glBindVertexArray(TerrainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TerrainVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TerrainIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/
}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}

//obsluga wejscia
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (spaceshipPos.x >= -6.57f && spaceshipPos.x <= -5.67f &&
			spaceshipPos.y >= 0.65f && spaceshipPos.y <= 1.75f &&
			spaceshipPos.z >= -0.3f && spaceshipPos.z <= 1.f) {
			if (animal_in_box) {
				printf("animal in hand\n");
				animal_in_box = false;
				animal_in_hand = true;
			}
			else if (animal_in_hand) {
				printf("animal in box\n");
				animal_in_box = true;
				animal_in_hand = false;
			}
		}
		if (spaceshipPos.x >= -1.0f && spaceshipPos.x <= 0.6f &&
			spaceshipPos.y >= 0.0f && spaceshipPos.y <= 3.0f &&
			spaceshipPos.z >= -4.6f && spaceshipPos.z <= 4.7f) {
			if (animal_in_aquarium) {
				printf("animal in hand\n");
				animal_in_aquarium = false;
				animal_in_hand = true;
			}
			else if (animal_in_hand) {
				printf("animal in aquarium\n");
				animal_in_aquarium = true;
				animal_in_hand = false;
			}
		}
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS) 
	{
		printf("spaceshipPos = glm::vec3(%ff, %ff, %ff);\n", spaceshipPos.x, spaceshipPos.y, spaceshipPos.z);
		printf("spaceshipDir = glm::vec3(%ff, %ff, %ff);\n", spaceshipDir.x, spaceshipDir.y, spaceshipDir.z);
	}
}


void processInput(GLFWwindow* window)
{
	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.f,1.f,0.f)));
	glm::vec3 spaceshipUp = glm::vec3(0.f, 1.f, 0.f);
	float angleSpeed = 0.05f * deltaTime * 60;
	float moveSpeed = 0.05f * deltaTime * 60;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float minX = -1000.f, maxX = 1000.f, minY = -1000.f, maxY = 1000.f, minZ = -1000.f, maxZ = 1000.f;
	//float minX = -6.f, maxX = -0.47f, minY = 0.65f, maxY = 2.5f, minZ = -4.4f, maxZ = 4.5f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm::vec3 newPos = spaceshipPos + spaceshipDir * moveSpeed;
		if (newPos.x > minX && newPos.x < maxX) spaceshipPos.x = newPos.x;
		if (newPos.y > minY && newPos.y < maxY) spaceshipPos.y = newPos.y;
		if (newPos.z > minZ && newPos.z < maxZ) spaceshipPos.z = newPos.z;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::vec3 newPos = spaceshipPos - spaceshipDir * moveSpeed;
		if (newPos.x > minX && newPos.x < maxX) spaceshipPos.x = newPos.x;
		if (newPos.y > minY && newPos.y < maxY) spaceshipPos.y = newPos.y;
		if (newPos.z > minZ && newPos.z < maxZ) spaceshipPos.z = newPos.z;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		spaceshipDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(spaceshipDir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		spaceshipDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(spaceshipDir, 0));
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		glm::vec3 newPos = spaceshipPos + spaceshipSide * moveSpeed;
		if (newPos.x > minX && newPos.x < maxX) spaceshipPos.x = newPos.x;
		if (newPos.y > minY && newPos.y < maxY) spaceshipPos.y = newPos.y;
		if (newPos.z > minZ && newPos.z < maxZ) spaceshipPos.z = newPos.z;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		glm::vec3 newPos = spaceshipPos - spaceshipSide * moveSpeed;
		if (newPos.x > minX && newPos.x < maxX) spaceshipPos.x = newPos.x;
		if (newPos.y > minY && newPos.y < maxY) spaceshipPos.y = newPos.y;
		if (newPos.z > minZ && newPos.z < maxZ) spaceshipPos.z = newPos.z;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glm::vec3 newPos = spaceshipPos + spaceshipUp * moveSpeed;
		if (newPos.x > minX && newPos.x < maxX) spaceshipPos.x = newPos.x;
		if (newPos.y > minY && newPos.y < maxY) spaceshipPos.y = newPos.y;
		if (newPos.z > minZ && newPos.z < maxZ) spaceshipPos.z = newPos.z;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		glm::vec3 newPos = spaceshipPos - spaceshipUp * moveSpeed;
		if (newPos.x > minX && newPos.x < maxX) spaceshipPos.x = newPos.x;
		if (newPos.y > minY && newPos.y < maxY) spaceshipPos.y = newPos.y;
		if (newPos.z > minZ && newPos.z < maxZ) spaceshipPos.z = newPos.z;
	}

	/*
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		spaceshipPos += spaceshipDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		spaceshipPos -= spaceshipDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		spaceshipDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(spaceshipDir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		spaceshipDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(spaceshipDir, 0));
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		spaceshipPos += spaceshipSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		spaceshipPos -= spaceshipSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		spaceshipPos += spaceshipUp * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		spaceshipPos -= spaceshipUp * moveSpeed;
	*/


	cameraPos = spaceshipPos - 0.5 * spaceshipDir + glm::vec3(0, 1, 0) * 0.2f;
	cameraDir = spaceshipDir;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		exposition -= 0.001;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		exposition += 0.001;

	glfwSetKeyCallback(window, key_callback);

	//cameraDir = glm::normalize(-cameraPos);
}


// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window))
	{
		//glClear(GL_COLOR_BUFFER_BIT);

		//updateBoids();
		//renderBoids();

		//glfwSwapBuffers(window);
		processInput(window);
		renderScene(window);
		glfwPollEvents();

		
	}
	
}
//}