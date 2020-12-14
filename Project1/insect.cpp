#define _USE_MATH_DEFINES

// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <glm/trigonometric.hpp>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#include "mesh.h"
#include "model.h"
#include "insect.h"
#include "textures.h"

const line Insect::walls[] = {
	{FLOOR_PT_1_X, FLOOR_PT_1_Y, FLOOR_PT_2_X, FLOOR_PT_2_Y},
	{FLOOR_PT_2_X, FLOOR_PT_2_Y, FLOOR_PT_3_X, FLOOR_PT_3_Y},
	{FLOOR_PT_3_X, FLOOR_PT_3_Y, FLOOR_PT_4_X, FLOOR_PT_4_Y},
	{FLOOR_PT_4_X, FLOOR_PT_4_Y, FLOOR_PT_5_X, FLOOR_PT_5_Y},
	{FLOOR_PT_5_X, FLOOR_PT_5_Y, FLOOR_PT_6_X, FLOOR_PT_6_Y},
	{FLOOR_PT_6_X, FLOOR_PT_6_Y, FLOOR_PT_1_X, FLOOR_PT_1_Y},
};


//using namespace std;


Insect::Insect() {};

Insect::Insect(std::vector<Mesh> tmp, GLfloat x, GLfloat y) {
	//std::vector<Mesh> tmp = Model::loadScene(file_name);
	head = Model(std::vector<Mesh>{tmp[20]}, SPDR_HEAD_TEX);
	eyes = Model(std::vector<Mesh>{tmp[18], tmp[19]}, SPDR_EYE_TEX);
	body = Model(std::vector<Mesh>{tmp[21]}, SPDR_BODY_TEX);
	shoulders = Model(std::vector<Mesh>{tmp[0], tmp[1], tmp[2], tmp[11], tmp[15], tmp[13]}, SPDR_SHOULDER_TEX);
	legs[0] = Leg(tmp[4], tmp[3]);
	legs[1] = Leg(tmp[6], tmp[5]);
	legs[2] = Leg(tmp[7], tmp[8]);
	legs[3] = Leg(tmp[10], tmp[9]);
	legs[4] = Leg(tmp[14], tmp[12]);
	legs[5] = Leg(tmp[17], tmp[16]);
	legs[0].switchSide();
	legs[3].switchSide();
	legs[4].switchSide();

	legs[0].alternateMotion();
	legs[4].alternateMotion();
	legs[2].alternateMotion();

	speed = 2.0f;

	this->x = x;
	this->y = y;
}

/*
0	shoulder-3-r
1	shoulder-2-r
2	shoulder-3-l
3	lwrleg-3-l
4	uprleg-3-l
5	lwrleg-3-r
6	uprleg-3-r
7	uprleg-2-r
8	lwrleg-2-r
9	lwrleg-2-l
10	uprleg-2-l
11	shoulder-2-l
12	Cone.004
13	shoulder-1-l
14	uprleg-1-l
15	shoulder-1-r
16	lwrleg-1-r
17	uprleg-1-r
18	Eye-l
19	Eye-r
20	Head
21	Body
*/
void Insect::generateObjectBufferMesh(GLuint shaderProgramID) {
	head.generateObjectBufferMesh(shaderProgramID);
	body.generateObjectBufferMesh(shaderProgramID);
	shoulders.generateObjectBufferMesh(shaderProgramID);
	eyes.generateObjectBufferMesh(shaderProgramID);
	for (int i = 0; i < 6; i++) {
		legs[i].generateObjectBufferMesh(shaderProgramID);
	}
}

void Insect::draw(mat4 parent, GLuint matrix_location, int texture_number_loc) {

	parent = rotate_z_deg(parent, 90 + glm::degrees(dir));
	parent = translate(parent, vec3(x, y, 0.7f));

	body.draw(parent, identity_mat4(), matrix_location, texture_number_loc);

	mat4 headMat = identity_mat4();
	head.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	eyes.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	shoulders.draw(parent, identity_mat4(), matrix_location, texture_number_loc);

	for (int i = 0; i < 6; i++) {
		legs[i].draw(parent, matrix_location, texture_number_loc);
	}
}

void Insect::update(float delta, std::vector<point> fellowsPos, int numFellows) {
	//rotate += 0.1f * delta * 50;
	vec3 direction = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 6; i++) {
		direction += normalise(avoidWall(walls[i])) * 5;
	}
	for (int i = 0; i < numFellows; i++) {
		direction += normalise(avoidFellows(fellowsPos[i].x, fellowsPos[i].y));
	}
	direction = normalise(direction);
	vec3 newDir = normalise((vec3(cos(dir), sin(dir), 0.0f) * 9 ) + direction);
	dir = glm::atan(newDir.v[1], newDir.v[0]);

	y += sin((dir)) * speed * delta;
	x += cos((dir)) * speed * delta;
	for (int i = 0; i < 6; i++) {
		legs[i].update(delta);
	}
}

void Insect::keypress(unsigned char key, int x, int y) {
	leg.keypress(key, x, y);
}

vec3 Insect::avoidWall(line l) {
	float m1 = (l.y2 - l.y1) / (l.x2 - l.x1);
	float c1 = (-(m1 * l.x1) + l.y1);

	float m2 = -1 / m1;
	float c2 = (-(m2 * x) + y);

	float intersectionX = (c2 - c1) / (m1 - m2);
	float intersectionY = m1 * intersectionX + c1;

	float bigEndpointX = l.x1;
	float bigEndpointY = l.y1;
	float smallEndpointX = l.x2;
	float smallEndpointY = l.y2;

	if (smallEndpointX > bigEndpointX) {
		bigEndpointX = l.x2;
		bigEndpointY = l.y2;
		smallEndpointX = l.x1;
		smallEndpointY = l.y1;
	}

	if (intersectionX > bigEndpointX) {
		intersectionX = bigEndpointX;
		intersectionY = bigEndpointY;
	}
	if (intersectionX < smallEndpointX) {
		intersectionX = smallEndpointX;
		intersectionY = smallEndpointY;
	}

	//printf("x: %.2f   y: %.2f\n", intersectionX, intersectionY);

	float dist = sqrtf((x - intersectionX) * (x - intersectionX) + (y - intersectionY) * (y - intersectionY));

	if (dist > 5) return vec3(0.0f, 0.0f, 0.0f);
	//printf("m1: %.2f   c2: %.2f\n", m1, c1);
	//printf("m2: %.2f   c2: %.2f\n", m2, c2);

	//printf("x: %.2f   y: %.2f\n", intersectionX, intersectionY);

	return vec3(x - intersectionX, y - intersectionY, 0.0f);

	//float dist = (m * x) - y + c;

	//int sideOfLine = 1;
	//if ((m * x) - y - c < 0) sideOfLine = -1;
	//if (dist < 0) {
	//	dist = -dist;
	//}

	//dist = dist / (sqrtf(m * m + 1));

	//return avoidFellows(intersectionX, intersectionY);
/*
	if (dist > 3) return vec3(0.0f, 0.0f, 0.0f);

	return vec3(sin(tan(m)) * sideOfLine, cos(tan(m)) * sideOfLine, 0.0f);*/
}

vec3 Insect::avoidFellows(float fellowX, float fellowY) {
	float dist = sqrtf((x - fellowX) * (x - fellowX) + (y - fellowY) * (y - fellowY));

	if (dist > 4) return vec3(0.0f, 0.0f, 0.0f);

	return vec3(x - fellowX, y - fellowY, 0.0f);
}