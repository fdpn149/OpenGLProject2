#include "pch.h"
#include "Scene.h"
#include <ctime>

#include <GL/glew.h>
#include <iostream>

#include "Config.h"

Scene::Scene()
	: mode(PickMode::ADD_FACE)
{
	/* OpenGL configs */

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glDisable(GL_CULL_FACE);


	/* Initialize shaders */

	shaders[ShaderTypes::MAIN]			= Shader("assets/shaders/picking.vs.glsl"	, "assets/shaders/picking.fs.glsl"	);
	shaders[ShaderTypes::SCREEN]		= Shader("assets/shaders/drawModel.vs.glsl"	, "assets/shaders/drawModel.fs.glsl");
	shaders[ShaderTypes::DRAW_POINT]	= Shader("assets/shaders/drawPoint.vs.glsl"	, "assets/shaders/drawPoint.fs.glsl");
	shaders[ShaderTypes::DRAW_FACE]		= Shader("assets/shaders/drawFace.vs.glsl"	, "assets/shaders/drawFace.fs.glsl"	);
	shaders[ShaderTypes::DRAW_LINE]		= Shader("assets/shaders/drawLine.vs.glsl"	, "assets/shaders/drawLine.fs.glsl"	);
	shaders[ShaderTypes::GRID]			= Shader("assets/shaders/grid.vs.glsl"		, "assets/shaders/grid.fs.glsl"		);

	
	/* Initialize mesh*/

	mesh.load(Config::MODEL_PATH + "UnionSphere.obj");


	/* Initialize camera */

	camera.setCameraView(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	projMat = glm::perspective(glm::radians(45.0f), (float)Config::SCR_W / Config::SCR_H, 0.1f, 100.0f);

	
	/* Initalize fbo */

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &fboTexId);
	glBindTexture(GL_TEXTURE_2D, fboTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, Config::SCR_W, Config::SCR_H, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboTexId, 0);


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

	
	/* Set shader uniforms */

	for (int iii = 0; iii < ShaderTypes::MAX_SHADER_NUM; ++iii)
	{
		shaders[iii].use();
		shaders[iii].setMat4("projMat", projMat);
		shaders[iii].setMat4("modelMat", glm::mat4(1.0f));
	}
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

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
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
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaders[ShaderTypes::MAIN].use();
	shaders[ShaderTypes::MAIN].setMat4("viewMat", camera.getViewMatrix());

	mesh.draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (mode)
	{
	case PickMode::ADD_FACE:
	case PickMode::DELETE_FACE:
		shaders[ShaderTypes::DRAW_LINE].use();
		shaders[ShaderTypes::DRAW_LINE].setMat4("viewMat", camera.getViewMatrix());
		mesh.drawLine();

		shaders[ShaderTypes::DRAW_FACE].use();
		shaders[ShaderTypes::DRAW_FACE].setMat4("viewMat", camera.getViewMatrix());
		mesh.drawSelecetedFaces();

		break;

	case PickMode::POINT:
		shaders[ShaderTypes::DRAW_POINT].use();
		shaders[ShaderTypes::DRAW_POINT].setMat4("viewMat", camera.getViewMatrix());
		mesh.drawPoint();
		break;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shaders[ShaderTypes::GRID].use();
	shaders[ShaderTypes::GRID].setMat4("viewMat", camera.getViewMatrix());
	mesh.draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shaders[ShaderTypes::SCREEN].use();
	shaders[ShaderTypes::SCREEN].setMat4("viewMat", camera.getViewMatrix());

	mesh.draw();

	glBindVertexArray(0);
}
