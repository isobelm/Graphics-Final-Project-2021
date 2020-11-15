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

#include <glm/gtc/type_ptr.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#include "mesh.h"
#include "model.h"



Model::Model() {};

Model::Model(std::vector<Mesh> meshesVec) {
	meshes = meshesVec;
};

std::vector<Mesh> Model::loadScene(const char* file_name) {
	std::vector<Mesh> meshes;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
	}
	else {

		printf("  %i materials\n", scene->mNumMaterials);
		printf("  %i meshes\n", scene->mNumMeshes);
		printf("  %i node children\n", scene->mRootNode->mNumChildren);
		printf("  %i textures\n", scene->mNumTextures);

		for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
			const aiMesh* mesh = scene->mMeshes[m_i];
			printf(mesh->mName.C_Str());
			printf("\n");
			printf("    %i vertices in mesh\n", mesh->mNumVertices);
			aiMatrix4x4 aiTransform = scene->mRootNode->mChildren[m_i]->mTransformation;
			mat4 transformation = mat4(aiTransform.a1, aiTransform.a2, aiTransform.a3, aiTransform.a4,
				aiTransform.b1, aiTransform.b2, aiTransform.b3, aiTransform.b4,
				aiTransform.c1, aiTransform.c2, aiTransform.c3, aiTransform.c4,
				aiTransform.d1, aiTransform.d2, aiTransform.d3, aiTransform.d4);
			meshes.push_back(Mesh(mesh, transformation, scene->mRootNode->mChildren[m_i]->mName.C_Str()));
			
		}
	}

	printf("  %i meshes\n", meshes.size());

	aiReleaseImport(scene);

	return meshes;
};

void Model::generateObjectBufferMesh(GLuint shaderProgramID) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].generateObjectBufferMesh(shaderProgramID);
	}
};

void Model::draw(mat4 parentTransform, mat4 childTransform, GLuint matrix_location) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(parentTransform, childTransform, matrix_location);
	}
};

void Model::setMeshes(std::vector<Mesh> newMeshes) {
	meshes = newMeshes;
};
