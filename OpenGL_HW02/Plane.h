#pragma once

class Plane
{
public:
	Plane();

	void draw();

private:
	void initBufferObjects();

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};

