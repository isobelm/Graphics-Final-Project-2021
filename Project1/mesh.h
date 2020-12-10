//#pragma once
#ifndef MESH_H
#define MESH_H
#include "mesh.h"
#include "assimp/scene.h"

#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

class Mesh
{
private:
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<unsigned int> indices;
	std::vector<vec2> mTextureCoords;
	GLuint loc1, loc2, loc3;
	GLuint vp_vbo, vn_vbo, vao, ebo, vt_vbo;

public:
	mat4 transformationMat;
	const char* name;
	Mesh();
	Mesh(const aiMesh* mesh, mat4 transformation, const char* name);
	void draw(mat4 parentTransform, mat4 childTransform, GLuint matrix_location, GLuint texture);
	void generateObjectBufferMesh(GLuint shaderProgramID);
};

#endif 