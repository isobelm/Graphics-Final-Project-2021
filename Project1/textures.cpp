#include "textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

GLuint textures[NUM_TEXTURES];

void loadTexture(int active_texture, GLuint texture, const char* texture_filename, const char* texture_name, int texture_number, GLuint shaderProgramID)
{
	glActiveTexture(active_texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texture_filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture: %d\n", texture_number);
	}

	glUniform1i(glGetUniformLocation(shaderProgramID, texture_name), texture_number);

	stbi_image_free(data);
}

void loadAllTextures(GLuint shaderProgramID)
{
	glGenTextures(NUM_TEXTURES, textures);

	loadTexture(GL_TEXTURE0, textures[SPDR_BODY_TEX], SPDR_BODY_TEX_FILE, SPDR_BODY_TEX_NAME, SPDR_BODY_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE1, textures[SPDR_EYE_TEX], SPDR_EYE_TEX_FILE, SPDR_EYE_TEX_NAME, SPDR_EYE_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE2, textures[SPDR_HEAD_TEX], SPDR_HEAD_TEX_FILE, SPDR_HEAD_TEX_NAME, SPDR_HEAD_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE3, textures[SPDR_SHOULDER_TEX], SPDR_SHOULDER_TEX_FILE, SPDR_SHOULDER_TEX_NAME, SPDR_SHOULDER_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE4, textures[SPDR_UPPR_LEG_TEX], SPDR_UPPR_LEG_TEX_FILE, SPDR_UPPR_LEG_TEX_NAME, SPDR_UPPR_LEG_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE5, textures[SPDR_LWR_LEG_TEX], SPDR_LWR_LEG_TEX_FILE, SPDR_LWR_LEG_TEX_NAME, SPDR_LWR_LEG_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE6, textures[PLAIN_TEX], PLAIN_TEX_FILE, PLAIN_TEX_NAME, PLAIN_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE7, textures[BIG_RUG_TEX], BIG_RUG_TEX_FILE, BIG_RUG_TEX_NAME, BIG_RUG_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE8, textures[FLOOR_TEX], FLOOR_TEX_FILE, FLOOR_TEX_NAME, FLOOR_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE9, textures[MOULDING_TEX], MOULDING_TEX_FILE, MOULDING_TEX_NAME, MOULDING_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE10, textures[SML_RUG_TEX], SML_RUG_TEX_FILE, SML_RUG_TEX_NAME, SML_RUG_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE11, textures[WALL_TEX], WALL_TEX_FILE, WALL_TEX_NAME, WALL_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE12, textures[LANTERN_TEX], LANTERN_TEX_FILE, LANTERN_TEX_NAME, LANTERN_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE13, textures[FIRE_TEX], FIRE_TEX_FILE, FIRE_TEX_NAME, FIRE_TEX, shaderProgramID);
	loadTexture(GL_TEXTURE14, textures[TREE_TEX], TREE_TEX_FILE, TREE_TEX_NAME, TREE_TEX, shaderProgramID);
}