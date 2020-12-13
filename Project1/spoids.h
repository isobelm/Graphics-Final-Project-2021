//#pragma once
#ifndef SPOIDS_H
#define SPOIDS_H
#include "model.h"
#include "insect.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

class Spoids {
public:

	Spoids();

	Spoids(const char* file_name);

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parent, GLuint matrix_location, int texture_number_loc);

	void update(float delta);

private:
	int num_spoids = 4;
	Insect spoids[4];
};

#endif