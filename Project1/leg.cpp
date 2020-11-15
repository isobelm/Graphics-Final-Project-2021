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

void Leg::draw(mat4 parentTransform, GLuint matrix_location) {
	mat4 hipTransform = identity_mat4();
	mat4 kneeTransform = identity_mat4();
	mat4 rotation = identity_mat4();
	//hipTransform = inverse(upperLeg.meshes[0].transformationMat) * hipTransform;
	hipTransform = translate(hipTransform, vec3(hip_transform_x, 0.0f, hip_transform_z));
	hipTransform = rotate_y_deg(hipTransform, cyclePos);
	hipTransform = translate(hipTransform, vec3(-hip_transform_x, 0.0f, -hip_transform_z));

	kneeTransform = translate(hipTransform, vec3(hip_transform_x * 2, 0.0f, hip_transform_z * 2));
	kneeTransform = rotate_y_deg(hipTransform, cyclePos);
	kneeTransform = translate(hipTransform, vec3(hip_transform_x * 2, 0.0f, hip_transform_z * 2));

	//hipTransform = hipTransform * upperLeg.meshes[0].transformationMat;
	upperLeg.draw(parentTransform, hipTransform, matrix_location);
	lowerLeg.draw(parentTransform, hipTransform, matrix_location);
};

void Leg::update() {
	cyclePos += motion;
	if (cyclePos > 50 || cyclePos < 0) motion =  -motion;
}

void Leg::keypress(unsigned char key, int x, int y) {
	/*switch (key) {
	case 'x':
		hip_transform_x += 0.05f;
		break;
	case 'z':
		hip_transform_x -= 0.05f;
		break;
	case 'c':
		hip_transform_y += 0.05f;
		break;
	case 'v':
		hip_transform_y -= 0.05f;
		break;
	}*/

	//printf("x:\t%f\n", hip_transform_x);
	//printf("y:\t%f\n", hip_transform_y);
}