//#pragma once
#ifndef House_H
#define House_H
//#define House_MESH "Models/House.dae"
#include "model.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

class House {
public:

	House();

	House(const char* file_name);

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parent, GLuint matrix_location, int texture_number_loc);

private:
	Model floor;
	Model wall;
	Model ceiling;
	Model doors;
	Model windows;
	Model moulding;
	Model bigRug;
	Model smlRug;
};

#endif