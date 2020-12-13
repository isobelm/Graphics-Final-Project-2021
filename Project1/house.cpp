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
#include "house.h"
#include "textures.h"

//using namespace std;


House::House() {};

House::House(const char* file_name) {
	std::vector<Mesh> tmp = Model::loadScene(file_name);
	wall = Model(std::vector<Mesh>{tmp[9]}, WALL_TEX);
	floor = Model(std::vector<Mesh>{tmp[8]}, FLOOR_TEX);
	ceiling = Model(std::vector<Mesh>{tmp[10], tmp[11]}, PLAIN_TEX);
	windows = Model(std::vector<Mesh>{tmp[0], tmp[1], tmp[2]}, PLAIN_TEX);
	doors = Model(std::vector<Mesh>{tmp[7], tmp[4], tmp[3], tmp[5]}, PLAIN_TEX);
	moulding = Model(std::vector<Mesh>{tmp[12], tmp[14], tmp[13]}, MOULDING_TEX);
	bigRug = Model(std::vector<Mesh>{tmp[6]}, BIG_RUG_TEX);
	smlRug = Model(std::vector<Mesh>{tmp[15]}, SML_RUG_TEX);
}

/*
0	window_window - 1
1	window.001_window - 2
2	window.002_window - 3
3	Cube_door
4	Cube.001_front - door - frame
5	Cylinder_door - handle
6	big_rug_big - rug
7	door_frame_door - frame
8	floor
9	hall_wall
10	hall - ceiling
11	main - ceiling
12	moulding_big_hall
13	moulding_main
14	moulding_small_hall
15	rug_sml - rug

*/

void House::generateObjectBufferMesh(GLuint shaderProgramID) {
	wall.generateObjectBufferMesh(shaderProgramID);
	floor.generateObjectBufferMesh(shaderProgramID);
	ceiling.generateObjectBufferMesh(shaderProgramID);
	windows.generateObjectBufferMesh(shaderProgramID);
	doors.generateObjectBufferMesh(shaderProgramID);
	moulding.generateObjectBufferMesh(shaderProgramID);
	bigRug.generateObjectBufferMesh(shaderProgramID);
	smlRug.generateObjectBufferMesh(shaderProgramID);
}

void House::draw(mat4 parent, GLuint matrix_location, int texture_number_loc) {
	wall.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	floor.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	ceiling.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	windows.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	doors.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	moulding.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	bigRug.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
	smlRug.draw(parent, identity_mat4(), matrix_location, texture_number_loc);
}


