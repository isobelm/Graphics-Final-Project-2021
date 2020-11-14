//#pragma once
#ifndef MESH_H
#define MESH_H


class Mesh
{
private:
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<unsigned int> indices;
	std::vector<vec2> mTextureCoords;
	GLuint loc1, loc2, loc3;
	GLuint vp_vbo, vn_vbo, vao, ebo;

public:
	Mesh();
	Mesh(const aiMesh* mesh);
	void draw();
	void generateObjectBufferMesh(GLuint shaderProgramID);
};

#endif 