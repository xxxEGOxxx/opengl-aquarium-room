#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "SOIL.h"
#include "Texture.h"

#include "Box.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

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

}

namespace texture {
	GLuint floorTexture;
	GLuint roomTexture;
	GLuint fishTexture;
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

glm::vec3 sunPos = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 sunDir = glm::vec3(0.f, 4.f, 0.f);
glm::vec3 sunColor = glm::vec3(0.9f, 0.6f, 0.7f) * 0.15;

glm::vec3 cameraPos = glm::vec3(0.479490f, 1.250000f, -2.124680f);
glm::vec3 cameraDir = glm::vec3(-0.354510f, 0.000000f, 0.935054f);


glm::vec3 spaceshipPos = glm::vec3(0.065808f, 1.250000f, -2.189549f);
glm::vec3 spaceshipDir = glm::vec3(-0.490263f, 0.000000f, 0.871578f);
GLuint VAO,VBO;

float aspectRatio = 1.f;

float exposition = 1.f;

glm::vec3 lampColor = glm::vec3(0.9f, 0.6f, 0.7f) * 5;
glm::vec3 lampColor2 = glm::vec3(1.0f, 0.2f, 0.2f) * 5;
glm::vec3 lampColor3 = glm::vec3(1.0f, 0.2f, 0.2f) * 5;

glm::vec3 pointlightPos = glm::vec3(-3.5, 2.8, 0);
glm::vec3 pointlightPos2 = glm::vec3(4.5, 2.8, 3);
glm::vec3 pointlightPos3 = glm::vec3(4.5, 2.8, -2.99);

glm::vec3 pointlightColor = glm::vec3(0.9, 0.6, 0.6) * 30;
glm::vec3 pointlightColor2 = glm::vec3(1, 0.2, 0.2) * 20;
glm::vec3 pointlightColor3 = glm::vec3(1, 0.2, 0.2) * 20;


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
	"./textures/skybox/space_rt.png",
	"./textures/skybox/space_up.png",
	"./textures/skybox/space_dn.png",
	"./textures/skybox/space_ft.png",
	"./textures/skybox/space_bk.png"
};



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
	Core::DrawContext(context);

}

void drawObjectPBRWithTexture(Core::RenderContext& context, glm::mat4 modelMatrix, GLuint textureID, float roughness, float metallic, float size) {
	glUseProgram(programTex);
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;


	glUniform1f(glGetUniformLocation(programTex, "size"), size);


	/*glm::mat4 transformation = modelMatrix;*/
	glUniformMatrix4fv(glGetUniformLocation(programTex, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(programTex, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	//glUniform3f(glGetUniformLocation(programTex, "lightPos"), pointlightPos.x, pointlightPos.y, pointlightPos.z);


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

	//For shadows
	glm::mat4 lightVP = glm::ortho(-3.f, 2.3f, -1.3f, 3.f, -1.0f, 40.0f) * glm::lookAt(sunPos, sunPos - sunDir, glm::vec3(0, 1, 0));
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

	glm::mat4 lightVPSun = glm::ortho(-3.f, 2.3f, -1.3f, 3.f, -1.0f, 40.0f) * glm::lookAt(sunPos, sunPos - sunDir, glm::vec3(0, 1, 0));
	renderShadowapSun(depthMapFBO, lightVPSun);

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
	drawObjectPBR(models::chairContext, glm::mat4(), glm::vec3(0.195239f, 0.37728f, 0.8f), 0.4f, 0.0f);
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
	//drawObjectPBR(models::landContext, glm::mat4(), glm::vec3(0.5f, 0.5f, 0.1f), 0.5f, 0.0f);

	//drawObjectPBR(models::fishContext, glm::mat4(), glm::vec3(0.7f, 0.2f, 0.1f), 0.5f, 0.0f);
	//drawObjectPBR(models::glassWallContext, glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f);


	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::normalize(glm::cross(spaceshipSide, spaceshipDir));
	glm::mat4 specshipCameraRotrationMatrix = glm::mat4({
		spaceshipSide.x,spaceshipSide.y,spaceshipSide.z,0,
		spaceshipUp.x,spaceshipUp.y,spaceshipUp.z ,0,
		-spaceshipDir.x,-spaceshipDir.y,-spaceshipDir.z,0,
		0.,0.,0.,1.,
		});

	drawObjectPBR(shipContext,
		glm::translate(spaceshipPos) * specshipCameraRotrationMatrix * glm::eulerAngleY(glm::pi<float>()) * glm::scale(glm::vec3(0.03f)),
		glm::vec3(0.3, 0.3, 0.5),
		0.2, 1.0
	);

	if (animal_in_hand) {
		drawObjectPBR(sphereContext,
			glm::translate(spaceshipPos) * glm::translate(glm::vec3(0, 0.06f, 0)) * specshipCameraRotrationMatrix * glm::eulerAngleY(glm::pi<float>()) * glm::scale(glm::vec3(0.05f)),
			glm::vec3(0.7f, 0.2f, 0.1f),
			0.5, 0.0
		);
	}

	if (animal_in_aquarium) {
		drawObjectPBR(sphereContext,
			glm::translate(glm::vec3(2.5f, 1.25f, 0.0f)) * glm::scale(glm::vec3(0.05f)) * glm::eulerAngleY(time) * glm::translate(glm::vec3(20.f, 0, 0)),
			glm::vec3(0.7f, 0.2f, 0.1f),
			0.5, 0.0
		);
	}

	if (animal_in_box) {
		drawObjectPBR(sphereContext,
			glm::translate(glm::vec3(-6.55f, 1.39f, 1.11f)) * glm::scale(glm::vec3(0.05f)),
			glm::vec3(0.7f, 0.2f, 0.1f),
			0.5, 0.0
		);
	}

	spotlightPos = spaceshipPos + 0.2 * spaceshipDir;
	spotlightConeDir = spaceshipDir;

	//draw texture with PBR

	drawObjectPBRWithTexture(models::floorContext, glm::mat4(), texture::floorTexture, 0.8f, 0.0f,5);
	drawObjectPBRWithTexture(models::roomContext, glm::mat4(), texture::roomTexture, 0.8f, 0.0f, 5);
	drawObjectPBRWithTexture(models::fishContext, glm::mat4(), texture::fishTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::landContext, glm::mat4(), texture::landTexture, 0.5f, 0.0f, 5);
	drawObjectPBRWithTexture(models::sofaBaseContext, glm::mat4(), texture::sofaBaseTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::sofaContext, glm::mat4(), texture::sofaTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::roofContext, glm::mat4(), texture::roofTexture, 0.5f, 0.0f, 5);
	drawObjectPBRWithTexture(models::fish2Context, glm::mat4(), texture::fishTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::fish3Context, glm::mat4(), texture::fishTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door1Context, glm::mat4(), texture::door1Texture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door2Context, glm::mat4(), texture::door2Texture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door3Context, glm::mat4(), texture::door3Texture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::doorhandleContext, glm::mat4(), texture::doorhandleTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door_next_toContext, glm::mat4(), texture::door_next_toTexture, 0.5f, 0.0f, 0);
	drawObjectPBRWithTexture(models::door_next_to_doorhandleContext, glm::mat4(), texture::door_next_to_doorhandleTexture, 0.5f, 0.0f, 0);

	//objects with textures that contain transparency should be drawn here (last)
	drawObjectPBRWithTexture(models::glassWallContext, glm::mat4(), texture::glassWallTexture, 0.8f, 0.0f, 5);

	//test depth buffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(programTest);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//Core::DrawContext(models::testContext);

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

	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_9_1.vert", "shaders/shader_9_1.frag");
	programTest = shaderLoader.CreateProgram("shaders/test.vert", "shaders/test.frag");
	programSun = shaderLoader.CreateProgram("shaders/shader_8_sun.vert", "shaders/shader_8_sun.frag");


	//loading models
	loadModelToContext("./models/sphere.obj", sphereContext);
	loadModelToContext("./models/spaceship.obj", shipContext);
	loadModelToContext("./models/Marble_Bust.obj", models::marbleBustContext);
	loadModelToContext("./models/Pencils_Cylinder.obj", models::pencilsContext);
	loadModelToContext("./models/plane.obj", models::floorContext);
	loadModelToContext("./models/room.obj", models::roomContext);
	loadModelToContext("./models/spaceship.obj", models::spaceshipContext);
	loadModelToContext("./models/sphere.obj", models::sphereContext);
	loadModelToContext("./models/test.obj", models::testContext);

	loadModelToContext("./models/Sofa_baseCube.obj", models::sofaContext);
	loadModelToContext("./models/Sofa_base.obj", models::sofaBaseContext);
	loadModelToContext("./models/Door1.obj", models::door1Context);
	loadModelToContext("./models/Door2.obj", models::door2Context);
	loadModelToContext("./models/Door3.obj", models::door3Context);
	loadModelToContext("./models/DoorCircle.obj", models::doorhandleContext);

	loadModelToContext("./models/Shelf.obj", models::shelfContext);
	loadModelToContext("./models/fish.obj", models::fishContext);
	loadModelToContext("./models/fish2.obj", models::fish2Context);
	loadModelToContext("./models/fish3.obj", models::fish3Context);
	loadModelToContext("./models/Glass_wall.obj", models::glassWallContext);
	loadModelToContext("./models/land.obj", models::landContext);
	loadModelToContext("./models/roof.obj", models::roofContext);

	loadModelToContext("./models/door_next_to.obj", models::door_next_toContext);
	loadModelToContext("./models/door_next_to_doorhandle.obj", models::door_next_to_doorhandleContext);

	//loading textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//textures that contain transparency should be loaded here
	texture::glassWallTexture = Core::LoadTexture("textures/glass.png");
	glDisable(GL_BLEND);
	programTex = shaderLoader.CreateProgram("shaders/shader_texture.vert", "shaders/shader_texture.frag");
	texture::glassWallTexture = Core::LoadTexture("textures/glass.jpg");
	texture::fishTexture = Core::LoadTexture("textures/fish.png");
	texture::roomTexture = Core::LoadTexture("textures/roof.jpg");
	texture::sofaBaseTexture = Core::LoadTexture("textures/sofa.jpg");
	texture::sofaTexture = Core::LoadTexture("textures/sofa.jpg");
	texture::landTexture = Core::LoadTexture("textures/grass.jpg");
	texture::floorTexture = Core::LoadTexture("textures/floor.png");
	texture::roofTexture = Core::LoadTexture("textures/roof.jpg");
	texture::door1Texture = Core::LoadTexture("textures/Door.jpg");
	texture::door2Texture = Core::LoadTexture("textures/Door.jpg");
	texture::door3Texture = Core::LoadTexture("textures/Door.jpg");
	texture::doorhandleTexture = Core::LoadTexture("textures/Door.jpg");
	texture::door_next_toTexture = Core::LoadTexture("textures/Door.jpg");
	texture::door_next_to_doorhandleTexture = Core::LoadTexture("textures/Door.jpg");
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
		if (cameraPos.x >= -6.57f && cameraPos.x <= -5.67f &&
			cameraPos.y >= 0.65f && cameraPos.y <= 1.75f &&
			cameraPos.z >= 0.27f && cameraPos.z <= 1.67f) {
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
		if (cameraPos.x >= -1.0f && cameraPos.x <= 0.6f &&
			cameraPos.y >= 0.0f && cameraPos.y <= 3.0f &&
			cameraPos.z >= -4.6f && cameraPos.z <= 4.7f) {
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

	cameraPos = spaceshipPos - 0.5 * spaceshipDir + glm::vec3(0, 1, 0) * 0.2f;
	cameraDir = spaceshipDir;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		exposition -= 0.05;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		exposition += 0.05;

	glfwSetKeyCallback(window, key_callback);

	//cameraDir = glm::normalize(-cameraPos);
}


// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		renderScene(window);
		glfwPollEvents();
	}
}
//}