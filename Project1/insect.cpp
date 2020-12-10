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
	std::vector<Mesh> tmp = Model::loadScene(file_name);
	//std::vector<Mesh> bodyMesh{ tmp.meshes[0] };
	//std::vector<Mesh>  legsMesh{ tmp.meshes[1] };
	//std::vector<Mesh>  headMesh{ tmp.meshes[2],tmp.meshes[3], tmp.meshes[4] };
	head = Model(std::vector<Mesh>{tmp[18], tmp[19], tmp[20]});
	//head.setMeshes(headMesh);
	body = Model(std::vector<Mesh>{tmp[2], tmp[5], tmp[7], tmp[11], tmp[15], tmp[13], tmp[21]});
	//body.setMeshes(bodyMesh);
	//legs.setMeshes(legsMesh);
	legs[0] = Leg(tmp[1], tmp[0]);
	legs[1] = Leg(tmp[4], tmp[3]); 
	legs[2] = Leg(tmp[6], tmp[8]);
	legs[3] = Leg(tmp[10], tmp[9]);
	legs[4] = Leg(tmp[14], tmp[12]);
	legs[5] = Leg(tmp[17], tmp[16]);
	legs[0].switchSide();
	legs[3].switchSide();
	legs[4].switchSide();

	legs[0].alternateMotion();
	legs[4].alternateMotion();
	legs[2].alternateMotion();
}

/*
0	lwrleg-3-r.001
1	uprleg-3-l
2	shoulder-3-l
3	lwrleg-3-r
4	uprleg-3-r
5	shoulder-3-r
6	uprleg-2-r
7	shoulder-2-r
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
	//legs.generateObjectBufferMesh(shaderProgramID);
	//leg.generateObjectBufferMesh(shaderProgramID);
	for (int i = 0; i < 6; i++) {
		legs[i].generateObjectBufferMesh(shaderProgramID);
	}
}

void Insect::draw(mat4 parent, GLuint matrix_location, GLuint texture) {
	printf("6\n");
	body.draw(parent, identity_mat4(), matrix_location, texture);
	printf("7\n");

	mat4 headMat = identity_mat4();/*
	headMat = rotate_z_deg(headMat, rotate);
	headMat = translate(headMat, vec3(0.0f, 0.0f, 0.0f));
	headMat = parent * headMat;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, headMat.m);*/
	//head.draw(parent, identity_mat4(), matrix_location, texture);

	/*mat4 legsMat = identity_mat4();
	legsMat = rotate_z_deg(legsMat, -rotate);
	legsMat = translate(legsMat, vec3(0.0f, 0.0f, 0.0f));
	legsMat = parent * legsMat;*/
	//glUniformMatrix4fv(matrix_location, 1, GL_FALSE, legsMat.m);
	//legs.draw(parent, matrix_location);
	for (int i = 0; i < 6; i++) {
		//legs[i].draw(parent, matrix_location, texture);
	}
	//leg.draw(parent, matrix_location);
}

void Insect::update(float delta) {
	rotate += 0.1f * delta * 50;
	//leg.update();
	for (int i = 0; i < 6; i++) {
		legs[i].update(delta);
	}
}

void Insect::keypress(unsigned char key, int x, int y) {
	leg.keypress(key, x, y);
}


