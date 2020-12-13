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
#include "lantern.h"
#include "textures.h"

//using namespace std;


Lantern::Lantern() {};

Lantern::Lantern(const char* file_name) {
	std::vector<Mesh> tmp = Model::loadScene(file_name);
	cage = Model(std::vector<Mesh>{tmp[0]}, LANTERN_TEX);
	fire = Model(std::vector<Mesh>{tmp[1]}, FIRE_TEX);
}


void Lantern::generateObjectBufferMesh(GLuint shaderProgramID) {
	cage.generateObjectBufferMesh(shaderProgramID);
	fire.generateObjectBufferMesh(shaderProgramID);
}

void Lantern::draw(mat4 parent, GLuint matrix_location, int texture_number_loc, int light_loc) {
	glUniform1i(light_loc, true);
	fire.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	glUniform1i(light_loc, false);
	cage.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
}


