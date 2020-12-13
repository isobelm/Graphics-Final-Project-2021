//#pragma once
#ifndef INSECT_H
#define INSECT_H
//#define INSECT_MESH "Models/insect.dae"
#include "model.h"
#include "leg.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

#define FLOOR_PT_1_X -0.3 
#define FLOOR_PT_1_Y -15.5 
#define FLOOR_PT_2_X -14.8 
#define FLOOR_PT_2_Y -17.3 
#define FLOOR_PT_3_X -14.9 
#define FLOOR_PT_3_Y 22.1 
#define FLOOR_PT_4_X 16.1 
#define FLOOR_PT_4_Y 21.2 
#define FLOOR_PT_5_X 13.6
#define FLOOR_PT_5_Y 4.5 
#define FLOOR_PT_6_X 1.6 
#define FLOOR_PT_6_Y 6.3 

struct line { float x1; float y1; float x2; float y2; };
struct point { float x; float y; };

class Insect {
public:

	static const line walls[6];

	Insect();

	Insect(std::vector<Mesh> tmp, GLfloat x, GLfloat y);

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parent, GLuint matrix_location, int texture_number_loc);

	void update(float delta, std::vector<point> fellowsPos, int numFellows);

	void keypress(unsigned char key, int x, int y);

	GLfloat x, y, dir, speed;

private:
	Model head;
	Model eyes;
	Model body;
	Model shoulders;
	Leg legs[6];
	Leg leg;
	GLfloat rotate = 0;

	//float distanceFromWall(float a, float b, line l);
	//vec3 getDirectiontoTurn(float dir, line l);
	vec3 avoidWall(line l);
	vec3 avoidFellows(float x, float y);

};

#endif