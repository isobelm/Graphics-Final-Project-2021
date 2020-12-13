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
	{FLOOR_PT_4_X, FLOOR_PT_4_Y, FLOOR_PT_1_X, FLOOR_PT_1_Y},
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
	//headTex = headTex;
	//bodyTex = bodyTex;
	//legTex = legTex;
	//shoulderTex = shoulderTex;
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

	parent = rotate_z_deg(parent, 90 - dir);
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

void Insect::update(float delta) {
	//rotate += 0.1f * delta * 50;
	vec3 direction = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		if (distanceFromWall(x, y, walls[i]) < 2) {
			direction += getDirectiontoTurn(dir, walls[i]);
		}
	}

	dir +=  0.1 * (-glm::atan(direction.v[1], direction.v[0]));
	/*
		if (direction < 0) dir += delta * 100;
		if (direction > 0) dir -=  delta * 100;*/

	y += sin(glm::radians(dir)) * speed * delta;
	x += cos(glm::radians(dir)) * speed * delta;
	for (int i = 0; i < 6; i++) {
		legs[i].update(delta);
	}
}

void Insect::keypress(unsigned char key, int x, int y) {
	leg.keypress(key, x, y);
}

float Insect::distanceFromWall(float a, float b, line l) {
	float m = (l.y2 - l.y1) / (l.x2 - l.x1);
	//printf("m: %.2f\n", m);

	float c = (-(m * l.x1) + l.y1);
	//printf("c: %.2f\n", c);

	float dist = (m * a) - b + c;
	//printf("x: %.2f   y: %.2f\n", l.x1, l.y1);
	//printf("abs: %.2f\n", dist);

	if (dist < 0) dist = -dist;
	//printf("abs: %.2f\n", dist);

	dist = dist / (sqrtf(m * m + 1));
	//printf("%.2f\n", dist);
	return dist;
}

vec3 Insect::getDirectiontoTurn(float dir, line l) {
	float m = (l.y2 - l.y1) / (l.x2 - l.x1);
	//printf("m: %.2f\n", m);

	float c = (-(m * l.x1) + l.y1);
	//printf("c: %.2f\n", c);

	float dist = (m * x) - y + c;
	//printf("x: %.2f   y: %.2f\n", l.x1, l.y1);
	//printf("abs: %.2f\n", dist);

	int sideOfLine = 1;
	if (dist < 0) {
		dist = -dist;
		sideOfLine = -1;
	}
	//printf("abs: %.2f\n", dist);

	dist = dist / (sqrtf(m * m + 1));

	if (dist > 2) return vec3(0.0f, 0.0f, 0.0f);

	printf("x: %.2f y: %.2f\n", cos(tan(m)) * sideOfLine, sin(tan(m)) * sideOfLine);

	return vec3(cos(tan(m)) * sideOfLine, sin(tan(m)) * sideOfLine, 0.0f);


	//float angle = tan((dir - m) / (1 + dir * m));
	//printf("%.2f\n", angle);

	//if (angle < 0) return 0;
	//else if (angle > 90) return 1;
	//else return -1;

}