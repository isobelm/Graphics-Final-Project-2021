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


Mesh::Mesh() {};

Mesh::Mesh(const aiMesh* mesh, mat4 transformation) {
	printf("    %i vertices in mesh\n", mesh->mNumVertices);
	mPointCount += mesh->mNumVertices;
	transformationMat = transformation;
	for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
		if (mesh->HasPositions()) {
			const aiVector3D* vp = &(mesh->mVertices[v_i]);
			mVertices.push_back(vec3(vp->x, vp->y, vp->z));
		}
		if (mesh->HasNormals()) {
			const aiVector3D* vn = &(mesh->mNormals[v_i]);
			mNormals.push_back(vec3(vn->x, vn->y, vn->z));
		}
		if (mesh->HasTextureCoords(0)) {
			const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
			mTextureCoords.push_back(vec2(vt->x, vt->y));
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		if (mesh->HasTangentsAndBitangents()) {
			//const aiVector3D* vtg = &(mesh->mTangents[v_i]);
			//const aiVector3D* vbtg = &(mesh->mBitangents[v_i]);

			/* You can extract tangents and bitangents here              */
			/* Note that you might need to make Assimp generate this     */
			/* data for you. Take a look at the flags that aiImportFile  */
			/* can take.                                                 */
		}


	};

};

void Mesh::draw(mat4 parentTransform, GLuint matrix_location) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);


	//headMat = rotate_z_deg(headMat, rotate);
	//headMat = translate(headMat, vec3(0.0f, 0.0f, 0.0f));
	mat4 transform = parentTransform * transformationMat;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, transform.m);

	//glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, &mIndices);
	//glDrawArrays(GL_TRIANGLES, 0, mPointCount);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

};

void Mesh::generateObjectBufferMesh(GLuint shaderProgramID) {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, mPointCount * sizeof(vec3), &mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);
	//unsigned int vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mPointCount * sizeof(vec3), &mNormals[0], GL_STATIC_DRAW);

	////	This is for texture coordinates which you don't currently need, so I have commented it out
	//unsigned int vt_vbo = 0;
	//glGenBuffers (1, &vt_vbo);
	//glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//glBufferData (GL_ARRAY_BUFFER, mesh_datamTextureCoords * sizeof (vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	////	This is for texture coordinates which you don't currently need, so I have commented it out
	//glEnableVertexAttribArray (loc3);
	//glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}
//#pragma endregion VBO_FUNCTIONS
//};