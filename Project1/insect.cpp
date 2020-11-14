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

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#include "mesh.h"
#include "model.h"
#include "insect.h"

//using namespace std;


Insect::Insect() {};

Insect::Insect(const char* file_name) {
	Model tmp = Model(file_name);
	//std::vector<Mesh> bodyMesh{ tmp.meshes[0] };
	//std::vector<Mesh>  legsMesh{ tmp.meshes[1] };
	//std::vector<Mesh>  headMesh{ tmp.meshes[2],tmp.meshes[3], tmp.meshes[4] };
	head = Model();
	//head.setMeshes(headMesh);
	body = tmp;
	//body.setMeshes(bodyMesh);
	//legs.setMeshes(legsMesh);
}

void Insect::generateObjectBufferMesh(GLuint shaderProgramID) {
	head.generateObjectBufferMesh(shaderProgramID);
	body.generateObjectBufferMesh(shaderProgramID);
	legs.generateObjectBufferMesh(shaderProgramID);
}

void Insect::draw(mat4 parent, GLuint matrix_location) {
	body.draw();
	mat4 headMat = identity_mat4();
	headMat = rotate_z_deg(headMat, rotate);
	headMat = translate(headMat, vec3(0.0f, 0.0f, 0.0f));
	headMat = parent * headMat;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, headMat.m);
	head.draw();

	mat4 legsMat = identity_mat4();
	legsMat = rotate_z_deg(legsMat, -rotate);
	legsMat = translate(legsMat, vec3(0.0f, 0.0f, 0.0f));
	legsMat = parent * legsMat;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, legsMat.m);
	legs.draw();
}

void Insect::update() {
	rotate += 0.1f;
}

