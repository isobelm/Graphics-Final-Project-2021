#ifndef TEXTURES_H
#define TEXTURES_H

#include <GL/glew.h>

//#pragma region TEXTURE NAMES
#define SPDR_BODY_TEX_FILE "Textures/body_test_tex.png"
#define SPDR_BODY_TEX_NAME "body_texture"
#define SPDR_EYE_TEX_FILE "Textures/Eye.png"
#define SPDR_EYE_TEX_NAME "eye_texture"
#define SPDR_HEAD_TEX_FILE "Textures/head.png"
#define SPDR_HEAD_TEX_NAME "head_texture"
#define SPDR_SHOULDER_TEX_FILE "Textures/shoulder-sml.png"
#define SPDR_SHOULDER_TEX_NAME "shoulder_texture"
#define SPDR_UPPR_LEG_TEX_FILE "Textures/uppr-leg-sml.png"
#define SPDR_UPPR_LEG_TEX_NAME "uppr_leg_texture"
#define SPDR_LWR_LEG_TEX_FILE "Textures/lwr-leg-sml.png"
#define SPDR_LWR_LEG_TEX_NAME "lwr_leg_texture"
#define PLAIN_TEX_FILE "Textures/plain.png"
#define PLAIN_TEX_NAME "plain_texture"
#define BIG_RUG_TEX_FILE "Textures/big-rug.png"
#define BIG_RUG_TEX_NAME "big_rug_texture"
#define FLOOR_TEX_FILE "Textures/floor.png"
#define FLOOR_TEX_NAME "floor_texture"
#define MOULDING_TEX_FILE "Textures/moulding.png"
#define MOULDING_TEX_NAME "moulding_texture"
#define SML_RUG_TEX_FILE "Textures/sml-rug.png"
#define SML_RUG_TEX_NAME "sml_rug_texture"
#define WALL_TEX_FILE "Textures/wall.png"
#define WALL_TEX_NAME "wall_texture"

#define SPDR_BODY_TEX 0
#define SPDR_EYE_TEX 1
#define SPDR_HEAD_TEX 2
#define SPDR_SHOULDER_TEX 3
#define SPDR_UPPR_LEG_TEX 4
#define SPDR_LWR_LEG_TEX 5
#define PLAIN_TEX 6
#define BIG_RUG_TEX 7
#define FLOOR_TEX 8
#define MOULDING_TEX 9
#define SML_RUG_TEX 10
#define WALL_TEX 11

//#pragma endregion TEXTURE NAMES

#define NUM_TEXTURES 12

extern GLuint textures[NUM_TEXTURES];

void loadAllTextures(GLuint shaderProgramID);


void loadTexture(int active_texture, GLuint texture, const char* texture_filename, const char* texture_name, int texture_number, GLuint shaderProgramID);

#endif
