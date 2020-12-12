//#pragma once
#ifndef House_H
#define House_H
//#define House_MESH "Models/House.dae"
#include "model.h"
#include "leg.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

class House {
public:

	House();

	House(const char* file_name,
		GLuint headTex,
		GLuint bodyTex,
		GLuint legTex,
		GLuint shoulderTex);

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parent, GLuint matrix_location, int texture_number_loc);

	void update(float delta);

	void keypress(unsigned char key, int x, int y);

private:
	//GLuint headTex;
	//GLuint bodyTex;
	//GLuint legTex;
	//GLuint shoulderTex;
	int headTex = 2;
	int bodyTex = 0;
	int eyeTex = 1;
	int shoulderTex = 3;
	Model head;
	Model eyes;
	Model body;
	Model shoulders;
	Leg legs[6];
	Leg leg;
	GLfloat rotate = 0;
};

#endif