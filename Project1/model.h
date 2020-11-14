//#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include "assimp/scene.h"

class Model {
public:
	std::vector<Mesh> meshes;

	Model();

	Model(const char* file_name);

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parentTransform, GLuint matrix_location);

	void setMeshes(std::vector<Mesh> meshes);

	//static aiNode loadScene(const char* file_name);

};

#endif // !MODEL_H
