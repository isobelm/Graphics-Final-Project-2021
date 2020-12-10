// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>


// Project includes
#include "maths_funcs.h"
#include "mesh.h"
#include "model.h"
#include "leg.h"


Leg::Leg() {};

Leg::Leg(Mesh upprLeg, Mesh lwrLeg) {
	upperLeg = upprLeg;
	lowerLeg = lwrLeg;
};

void Leg::generateObjectBufferMesh(GLuint shaderProgramID) {
	upperLeg.generateObjectBufferMesh(shaderProgramID);
	lowerLeg.generateObjectBufferMesh(shaderProgramID);
};

void Leg::draw(mat4 parentTransform, GLuint matrix_location, GLuint texture) {
	mat4 hipTransform = identity_mat4();
	mat4 kneeTransform = identity_mat4();
	hipTransform = translate(hipTransform, vec3(hip_transform_x, 0.0f, hip_transform_z));
	kneeTransform = translate(kneeTransform, vec3(hip_transform_x + knee_transform_x, 0.0f, hip_transform_z + knee_transform_z));
	kneeTransform = rotate_z_deg(kneeTransform, -cyclePos_x);

	if (cyclePos_y > 0) {
		hipTransform = rotate_y_deg(hipTransform, cyclePos_y * rotate_dir);
		kneeTransform = rotate_y_deg(kneeTransform, cyclePos_y * rotate_dir);
	}
	hipTransform = rotate_x_deg(hipTransform, cyclePos_x * rotate_dir);
	hipTransform = translate(hipTransform, vec3(-hip_transform_x, 0.0f, -hip_transform_z));
	kneeTransform = translate(kneeTransform, vec3(-knee_transform_x - hip_transform_x, 0.0f, -knee_transform_z - hip_transform_z));

	//hipTransform = hipTransform * upperLeg.meshes[0].transformationMat;
	upperLeg.draw(parentTransform, hipTransform, matrix_location, texture);
	lowerLeg.draw(parentTransform, kneeTransform, matrix_location, texture);
};

void Leg::update(float delta) {
	cyclePos_y += motion_y * delta * 50;
	//printf("%.2f\n", cyclePos_y);
	if (cyclePos_y >= cycleLength || cyclePos_y <= -cycleLength) {
		motion_y = -motion_y;
		//printf("y:\t%f\n", cyclePos_y);
		//printf("x:\t%f\n", cyclePos_x);
	}
	cyclePos_x += motion_x * delta * 50;
	//printf("%.2f\n", cyclePos_x);

	if (cyclePos_x >= cycleLength || cyclePos_x <= -cycleLength) {
		motion_x = -motion_x;
		//printf("y:\t%f\n", cyclePos_y);
		//printf("x:\t%f\n", cyclePos_x);
	}

}

void Leg::switchSide() {
	knee_transform_x = -knee_transform_x;
	hip_transform_x = -hip_transform_x;
	rotate_dir = -rotate_dir;
	cyclePos_x = -cyclePos_x;
	motion_x = -motion_x;
}

void Leg::alternateMotion() {
	motion_x = -motion_x;
	cyclePos_x = -cyclePos_x;
	motion_y = -motion_y;
}

void Leg::keypress(unsigned char key, int x, int y) {
}