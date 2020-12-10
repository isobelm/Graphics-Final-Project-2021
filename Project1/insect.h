//#pragma once
#ifndef INSECT_H
#define INSECT_H
#define INSECT_MESH "Models/insect.dae"
#include "model.h"
#include "leg.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

class Insect {
public:

	Insect();

	Insect(const char* file_name);

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parent, GLuint matrix_location, GLuint texture);

	void update(float delta);

	void keypress(unsigned char key, int x, int y);

private:
	Model head;
	Model body;
	Leg legs[6];
	Leg leg;
	GLfloat rotate = 0;
};

#endif