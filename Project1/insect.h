//#pragma once
#ifndef INSECT_H
#define INSECT_H
#define INSECT_MESH "insect.dae"
#include "model.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>
class Insect {
public:
	Model head;
	Model body;
	Model legs;
	GLfloat rotate = 0;

	Insect();

	Insect(const char* file_name);

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parent, GLuint matrix_location);

	void update();
};

#endif