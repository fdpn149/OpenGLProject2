#include "pch.h"
#include "Scene.h"
#include <ctime>

#include <GL/glew.h>
#include <iostream>

Scene::Scene()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glDisable(GL_CULL_FACE);

	GLint _viewport[4];
	glGetIntegerv(GL_VIEWPORT, _viewport);

	mode = 0;

	shader = new Shader("assets/shaders/picking.vs.glsl", "assets/shaders/picking.fs.glsl");
	screenShader = new Shader("assets/shaders/drawModel.vs.glsl", "assets/shaders/drawModel.fs.glsl");
	drawPointShader = new Shader("assets/shaders/drawPoint.vs.glsl", "assets/shaders/drawPoint.fs.glsl");
	drawFaceShader = new Shader("assets/shaders/drawFace.vs.glsl", "assets/shaders/drawFace.fs.glsl");
	gridShader = new Shader("assets/shaders/grid.vs.glsl", "assets/shaders/grid.fs.glsl");

	camera = new Camera(glm::vec3(0, 0, 3.0f), glm::radians(0.0f), glm::radians(0.0f), glm::vec3(0, 1.0f, 0));

	mesh = new Mesh;

	projMat = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 100.0f);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, WIDTH, HEIGHT, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureColorbuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	gridShader->use();
	gridShader->setMat4("projMat", projMat);
	gridShader->setMat4("modelMat", glm::mat4(1.0f));

	shader->use();
	shader->setMat4("projMat", projMat);
	shader->setMat4("modelMat", glm::mat4(1.0f));

	screenShader->use();
	screenShader->setMat4("projMat", projMat);
	screenShader->setMat4("modelMat", glm::mat4(1.0f));

	drawFaceShader->use();
	drawFaceShader->setMat4("projMat", projMat);
	drawFaceShader->setMat4("modelMat", glm::mat4(1.0f));

	glm::vec3 pointColor(0.0, 1.0, 0);
	drawPointShader->use();
	drawPointShader->setVec3("pointColor", pointColor);
	drawPointShader->setMat4("projMat", projMat);
	drawPointShader->setMat4("modelMat", glm::mat4(1.0f));
}

void Scene::pickingFace(uint faceID)
{
	glm::vec3 faceColor = glm::vec3(1.0, 0, 0);

	drawFaceShader->use();
	drawFaceShader->setVec3("faceColor", faceColor);

	mesh->addSelectedFace(faceID);
}

void Scene::deleteFace(uint faceID)
{
	mesh->deleteSelectedFace(faceID);
}

void Scene::pickingPoint(float depthValue, uint faceID, int x, int y)
{
	glm::vec4 viewport(0, 0, WIDTH, HEIGHT);
	glm::vec3 windowPos(x, y, depthValue);
	glm::vec3 worldPos = glm::unProject(windowPos, camera->getViewMatrix(), projMat, viewport);

	printf("Position:\t(%f, %f, %f)\n\n", worldPos.x, worldPos.y, worldPos.z);


	TriMesh::Point closestPoint = mesh->findClosestPoint(faceID, worldPos);
	
	mesh->setPointPosition(glm::vec3(closestPoint[0], closestPoint[1], closestPoint[2]));

	drawPointShader->use();
	drawPointShader->setMat4("viewMat", camera->getViewMatrix());
	glUseProgram(0);

}

Scene::~Scene()
{
	delete shader, screenShader, drawPointShader, drawFaceShader;
	delete camera;
	delete mesh;
}

void Scene::changeDirection(int x, int y)
{
	camera->onMousePositionChanged(-x, -y, 0);
}

void Scene::mouseUp()
{
	camera->firstMouse = true;
}

void Scene::changePosition(wchar_t key, clock_t deltaTime)
{
	camera->onKeyPressed(key, deltaTime);
}

void Scene::changePosition(int x, int y)
{
	camera->onMousePositionChanged(x, y, 1);
}

void Scene::changeDistance(int delta)
{
	camera->onMouseWheelScroll(delta);
}


void Scene::picking(int x, int y)
{
	y = HEIGHT - y;

	printf("Mouse Position:\t(%d, %d)\n", x, y);


	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	float depthValue = 0;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue);

	printf("depthValue:\t%f\n", depthValue);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	uint faceID;
	glReadPixels(x, y - 1, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &faceID);
	faceID--;
	printf("currentFaceID:\t%u\n\n", faceID);

	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	switch (mode)
	{
	case 0:
		pickingFace(faceID);
		break;
	case 1:
		deleteFace(faceID);
		break;
	case 2:
		pickingPoint(depthValue, faceID, x, y);
		break;
	}

}

void Scene::draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->use();
	shader->setMat4("viewMat", camera->getViewMatrix());

	mesh->draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (mode)
	{
	case 0:
	case 1:
		drawFaceShader->use();
		drawFaceShader->setMat4("viewMat", camera->getViewMatrix());
		mesh->drawFace();
		break;
	case 2:
		drawPointShader->use();
		drawPointShader->setMat4("viewMat", camera->getViewMatrix());
		mesh->drawPoint();
		break;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	gridShader->use();
	gridShader->setMat4("viewMat", camera->getViewMatrix());
	mesh->draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	screenShader->use();
	screenShader->setMat4("viewMat", camera->getViewMatrix());

	mesh->draw();

	glBindVertexArray(0);
}
