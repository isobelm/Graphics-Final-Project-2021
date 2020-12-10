//#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include "assimp/scene.h"

#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>


class Model {
public:
	//static vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	std::vector<Mesh> meshes;

	Model();

	Model(std::vector<Mesh> meshesVec);
	//Model(const char* file_name);

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parentTransform, mat4 childTransform, GLuint matrix_location, GLuint texture);

	void setMeshes(std::vector<Mesh> meshes);

	static std::vector<Mesh> loadScene(const char* file_name);

};

#endif // !MODEL_H
