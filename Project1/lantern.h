//#pragma once
#ifndef Lantern_H
#define Lantern_H
//#define Lantern_MESH "Models/Lantern.dae"
#include "model.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

class Lantern {
public:

	Lantern();

	Lantern(const char* file_name);

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parent, GLuint matrix_location, int texture_number_loc, int light_loc);

private:
	Model cage;
	Model fire;
};

#endif
