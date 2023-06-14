#include "pch.h"
#include "Scene.h"
#include <ctime>
#include <chrono>

#include <GL/glew.h>
#include <iostream>

#include "Config.h"

Scene::Scene()
	: lightPos(glm::vec3(0.2f, 1.0f, 0.3f))
	, light(Light(glm::vec3(0.0f) - glm::vec3(0.2f, 1.0f, 0.3f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)))
	, mode(PickMode::ADD_FACE)
	, shadowMapLength(2048)
{
	/* OpenGL configs */

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glDisable(GL_CULL_FACE);


	/* Initialize shaders */

	shaders[ShaderTypes::PICK]			= Shader("assets/shaders/picking.vs.glsl"	, "assets/shaders/picking.fs.glsl"	);
	shaders[ShaderTypes::MODEL]			= Shader("assets/shaders/drawModel.vs.glsl"	, "assets/shaders/drawModel.fs.glsl");
	shaders[ShaderTypes::DRAW_POINT]	= Shader("assets/shaders/drawPoint.vs.glsl"	, "assets/shaders/drawPoint.fs.glsl");
	shaders[ShaderTypes::DRAW_FACE]		= Shader("assets/shaders/drawFace.vs.glsl"	, "assets/shaders/drawFace.fs.glsl"	);
	shaders[ShaderTypes::GRID]			= Shader("assets/shaders/grid.vs.glsl"		, "assets/shaders/grid.fs.glsl"		);
	shaders[ShaderTypes::PLANE]			= Shader("assets/shaders/plane.vs.glsl"		, "assets/shaders/plane.fs.glsl"	);
	shaders[ShaderTypes::SHADOW_MAP]	= Shader("assets/shaders/shadowMap.vs.glsl" , "assets/shaders/shadowMap.fs.glsl");
	shaders[ShaderTypes::SKYBOX]		= Shader("assets/shaders/skybox.vs.glsl"	, "assets/shaders/skybox.fs.glsl"	);
	

	/* Initialize mesh*/

	mesh.loadMesh(Config::MODEL_PATH + "armadillo.obj");


	/* Initialze skybox */
	skybox.loadTexture(Config::TEXTURE_PATH + "skybox/");


	/* Initialize camera */

	camera.setCameraView(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	projMat = glm::perspective(glm::radians(45.0f), (float)Config::SCR_W / Config::SCR_H, 0.1f, 100.0f);

	
	/* Initalize picking fbo */

	glGenFramebuffers(1, &pickingFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFbo);

	glGenTextures(1, &pickingFboTexId);
	glBindTexture(GL_TEXTURE_2D, pickingFboTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, Config::SCR_W, Config::SCR_H, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, pickingFboTexId, 0);


	/* Initalize rbo */

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Config::SCR_W, Config::SCR_H);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	/* Initialize shadow map fbo */

	glGenFramebuffers(1, &shadowMapFbo);

	glGenTextures(1, &shadowMapFboTexId);
	glBindTexture(GL_TEXTURE_2D, shadowMapFboTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapLength, shadowMapLength, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Prevents darkness outside the frustrum
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapFboTexId, 0);

	// Needed since we don't touch the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Matrices needed for the light's perspective
	glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
	glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjection = orthgonalProjection * lightView;

	shaders[ShaderTypes::SHADOW_MAP].use();
	shaders[ShaderTypes::SHADOW_MAP].setMat4("lightProjMat", lightProjection);

	shaders[ShaderTypes::PLANE].use();
	shaders[ShaderTypes::PLANE].setMat4("lightProjMat", lightProjection);

	// Test shadow map
	quad.setTexId(shadowMapFboTexId);
	

	/* Set shader uniforms */

	for (int iii = 0; iii < ShaderTypes::MAX_SHADER_NUM; ++iii)
	{
		shaders[iii].use();
		shaders[iii].setMat4("projMat", projMat);
		shaders[iii].setMat4("modelMat", glm::mat4(1.0f));
	}

	/* Draw shadow map */
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glViewport(0, 0, shadowMapLength, shadowMapLength);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	shaders[ShaderTypes::SHADOW_MAP].use();
	shaders[ShaderTypes::SHADOW_MAP].setMat4("modelMat", mesh.getModelMat());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	mesh.draw();
	plane.draw(shaders[ShaderTypes::SHADOW_MAP]);

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void Scene::pickFace(unsigned int faceId)
{
	glm::vec3 faceColor = glm::vec3(1.0f, 0.0f, 0.0f);

	shaders[ShaderTypes::DRAW_FACE].use();
	shaders[ShaderTypes::DRAW_FACE].setVec3("faceColor", faceColor);

	mesh.addFaceToSelectedById(faceId);
}

void Scene::deleteFace(unsigned int faceId)
{
	mesh.deleteFaceFromSelectedById(faceId);
}

void Scene::pickingPoint(float depthValue, unsigned int faceId, int x, int y)
{
	GLint _viewport[4];
	glGetIntegerv(GL_VIEWPORT, _viewport);

	glm::vec4 viewport(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);
	glm::vec3 windowPos(x, y, depthValue);
	glm::vec3 worldPos = glm::unProject(windowPos, camera.getViewMatrix(), projMat, viewport);

#ifdef DEBUG
	printf("Position:\t(%f, %f, %f)\n\n", worldPos.x, worldPos.y, worldPos.z);
#endif // DEBUG


	TriMesh::Point closestPoint = mesh.findClosestPoint(faceId, worldPos);

	mesh.setPointPosition(glm::vec3(closestPoint[0], closestPoint[1], closestPoint[2]));

	shaders[ShaderTypes::DRAW_POINT].use();
	shaders[ShaderTypes::DRAW_POINT].setVec3("pointColor", glm::vec3(0.0f, 1.0f, 0.0f));
	shaders[ShaderTypes::DRAW_POINT].setMat4("viewMat", camera.getViewMatrix());
	glUseProgram(0);
}


void Scene::pick(int x, int y)
{
	y = Config::SCR_H - y;

	glBindFramebuffer(GL_FRAMEBUFFER, pickingFbo);
	float depthValue = 0;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	unsigned int faceID;
	glReadPixels(x, y - 1, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &faceID);
	--faceID;
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#ifdef DEBUG
	printf("Mouse Position:\t(%d, %d)\n", x, y);
	printf("depthValue:\t%f\n", depthValue);
	printf("currentFaceID:\t%u\n\n", faceID);
#endif

	switch (mode)
	{
	case PickMode::ADD_FACE:
		pickFace(faceID);
		break;
	case PickMode::DELETE_FACE:
		deleteFace(faceID);
		break;
	case PickMode::POINT:
		pickingPoint(depthValue, faceID, x, y);
		break;
	}
}

void Scene::draw()
{
	/* Draw skybox */
	shaders[ShaderTypes::SKYBOX].use();
	shaders[ShaderTypes::SKYBOX].setMat4("viewMat", glm::mat4(glm::mat3(camera.getViewMatrix())));

	shaders[ShaderTypes::SKYBOX].setInt("skybox", 0);
	glActiveTexture(GL_TEXTURE0);

	skybox.draw(shaders[ShaderTypes::SKYBOX]);


	/* Draw picking frame buffer */

	glBindFramebuffer(GL_FRAMEBUFFER, pickingFbo);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaders[ShaderTypes::PICK].use();
	shaders[ShaderTypes::PICK].setMat4("viewMat", camera.getViewMatrix());
	shaders[ShaderTypes::PICK].setMat4("modelMat", mesh.getModelMat());

	mesh.draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	switch (mode)
	{
	case PickMode::ADD_FACE:
	case PickMode::DELETE_FACE:

		/* Draw selected faces */

		shaders[ShaderTypes::DRAW_FACE].use();
		shaders[ShaderTypes::DRAW_FACE].setMat4("viewMat", camera.getViewMatrix());
		shaders[ShaderTypes::DRAW_FACE].setMat4("modelMat", mesh.getModelMat());
		
		mesh.drawSelected(shaders[ShaderTypes::DRAW_FACE]);

		break;

	case PickMode::POINT:

		/* Draw selected point */

		shaders[ShaderTypes::DRAW_POINT].use();
		shaders[ShaderTypes::DRAW_POINT].setMat4("viewMat", camera.getViewMatrix());

		mesh.drawPoint();
		
		break;
	}


	/* Draw model grid */

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shaders[ShaderTypes::GRID].use();
	shaders[ShaderTypes::GRID].setMat4("viewMat", camera.getViewMatrix());
	shaders[ShaderTypes::GRID].setMat4("modelMat", mesh.getModelMat());

	mesh.draw();


	/* Draw model */

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shaders[ShaderTypes::MODEL].use();
	shaders[ShaderTypes::MODEL].setMat4("viewMat", camera.getViewMatrix());
	shaders[ShaderTypes::MODEL].setMat4("modelMat", mesh.getModelMat());

	mesh.draw();


	/* Draw plane */

	shaders[ShaderTypes::PLANE].use();
	shaders[ShaderTypes::PLANE].setMat4("viewMat", camera.getViewMatrix());
	shaders[ShaderTypes::PLANE].setVec3("cameraPos", camera.getPosition());

	shaders[ShaderTypes::PLANE].setInt("shadowMap", 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowMapFboTexId);

	light.setLight2Shader(shaders[ShaderTypes::PLANE]);

	plane.draw(shaders[ShaderTypes::PLANE]);

	glBindTexture(GL_TEXTURE_2D, 0);
	

	///* Draw quad */
	//shaders[ShaderTypes::PLANE].use();
	//shaders[ShaderTypes::PLANE].setMat4("viewMat", camera.getViewMatrix());
	//shaders[ShaderTypes::PLANE].setVec3("cameraPos", camera.getPosition());

	//light.setLight2Shader(shaders[ShaderTypes::PLANE]);
	//quad.draw(shaders[ShaderTypes::PLANE]);


	glBindVertexArray(0);
}
