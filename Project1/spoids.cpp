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
#include "spoids.h"
#include "insect.h"
#include "textures.h"

//using namespace std;


Spoids::Spoids() {};

Spoids::Spoids(const char* file_name) {
	std::vector<Mesh> tmp = Model::loadScene(file_name);
	spoids[0] = *new Insect(tmp, 7, 13);
	spoids[1] = *new Insect(tmp, -7, 13);
	spoids[2] = *new Insect(tmp, -7, 1.3);
	spoids[3] = *new Insect(tmp, -7, 9);

	/*for (int i = 0; i < num_spoids; i++) {
		
	}*/
}

void Spoids::generateObjectBufferMesh(GLuint shaderProgramID) {
	for (int i = 0; i < num_spoids; i++) {
		spoids[i].generateObjectBufferMesh(shaderProgramID);
	}
}

void Spoids::draw(mat4 parent, GLuint matrix_location, int texture_number_loc) {
	for (int i = 0; i < num_spoids; i++) {
		spoids[i].draw(parent, matrix_location, texture_number_loc);
	}
}

void Spoids::update(float delta) {
	for (int i = 0; i < num_spoids; i++) {
		std::vector<point> fellowsPos;
		for (int j = 0; j < num_spoids; j++) {
			if (j != i) {
				fellowsPos.push_back(point{ spoids[j].x, spoids[j].y });
			}
		}
		spoids[i].update(delta, fellowsPos, num_spoids - 1);
	}
}


