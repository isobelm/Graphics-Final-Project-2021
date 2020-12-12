#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
#include <glm/trigonometric.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#include "insect.h"

#define MESH_NAME "Models/insect.dae"

#pragma region TEXTURE NAMES
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
#pragma endregion TEXTURE NAMES

using namespace std;
GLuint shaderProgramID;

Insect insect = Insect();
Model ground, door, lantern, house;
unsigned int mesh_vao = 0;
int width = 800;
int height = 600;

GLfloat rotate_y = 0.0f;
GLfloat rotate_view_x = -70.0f, rotate_view_z = 0.0f;
GLfloat view_x = 0.0f, view_y = 15.0f;

GLuint textures[12];


// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		std::cerr << "Error creating shader..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024] = { '\0' };
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling "
			<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader program: " << InfoLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		std::cerr << "Error creating shader program..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "simpleVertexShader.txt", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "simpleFragmentShader.txt", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS


void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.76f, 0.85f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);


	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
	int texture_number_loc = glGetUniformLocation(shaderProgramID, "texture_index");


	// Root of the Hierarchy
	mat4 view = identity_mat4();
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	mat4 model = identity_mat4();
	mat4 groundTransformation = identity_mat4();
	mat4 houseTransformation = identity_mat4();
	mat4 lanternTransformation = identity_mat4();
	model = translate(model, vec3(3.0f, 0.0f, 0.7f));
	model = rotate_z_deg(model, -rotate_y);

	groundTransformation = scale(groundTransformation, vec3(100, 100, 100));
	houseTransformation = scale(houseTransformation, vec3(3, 3, 3));

	view = translate(view, vec3(view_x, view_y, -5));
	lanternTransformation = scale(lanternTransformation, vec3(0.35, 0.35, 0.35));
	lanternTransformation = translate(lanternTransformation, vec3(-0.6, 2, 0));
	lanternTransformation = rotate_z_deg(lanternTransformation, -rotate_view_z);

	lanternTransformation = translate(lanternTransformation, vec3(-view_x, -view_y, 4));
	//doorTransformation = translate(doorTransformation, vec3(view_x, view_y, 0));
	//lanternTransformation = translate(lanternTransformation, vec3(5, -5, -5));
	view = rotate_z_deg(view, rotate_view_z);
	view = rotate_x_deg(view, rotate_view_x);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	//glBindTexture(GL_TEXTURE_2D, textures[SPDR_BODY_TEX]);
	insect.draw(model, matrix_location, texture_number_loc);
	house.draw(identity_mat4(), identity_mat4(), matrix_location, texture_number_loc, 8);
	//ground.draw(groundTransformation, identity_mat4(), matrix_location, textures[SPDR_BODY_TEX]);
	//door.draw(doorTransformation, identity_mat4(), matrix_location, textures[SPDR_BODY_TEX]);
	//lantern.draw(lanternTransformation, identity_mat4(), matrix_location, textures[SPDR_BODY_TEX]);

	glutSwapBuffers();
}


void updateScene() {

	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	// Rotate the model slowly around the y axis at 20 degrees per second
	rotate_y += 20.0f * delta;
	rotate_y = fmodf(rotate_y, 360.0f);

	insect.update(delta);

	// Draw the next frame
	glutPostRedisplay();
}

void loadTexture(int active_texture, GLuint texture, const char* texture_filename, const char* texture_name, int texture_number)
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
		std::cout << "Failed to load texture" << texture_number << std::endl;
	}

	glUniform1i(glGetUniformLocation(shaderProgramID, texture_name), texture_number);

	stbi_image_free(data);
}

void init()
{
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();

	insect = Insect(MESH_NAME, textures[SPDR_HEAD_TEX], textures[SPDR_BODY_TEX], textures[SPDR_EYE_TEX], textures[SPDR_SHOULDER_TEX]);
	house = Model(Model::loadScene("Models/house_full.obj"));
	//door = Model(Model::loadScene("Models/front_door.dae"));
	//lantern = Model(Model::loadScene("Models/lantern.dae"));

	insect.generateObjectBufferMesh(shaderProgramID);
	house.generateObjectBufferMesh(shaderProgramID);
	//door.generateObjectBufferMesh(shaderProgramID);
	//lantern.generateObjectBufferMesh(shaderProgramID);

	glGenTextures(12, textures);

	loadTexture(GL_TEXTURE0, textures[SPDR_BODY_TEX], SPDR_BODY_TEX_FILE, SPDR_BODY_TEX_NAME, SPDR_BODY_TEX);
	loadTexture(GL_TEXTURE1, textures[SPDR_EYE_TEX], SPDR_EYE_TEX_FILE, SPDR_EYE_TEX_NAME, SPDR_EYE_TEX);
	loadTexture(GL_TEXTURE2, textures[SPDR_HEAD_TEX], SPDR_HEAD_TEX_FILE, SPDR_HEAD_TEX_NAME, SPDR_HEAD_TEX);
	loadTexture(GL_TEXTURE3, textures[SPDR_SHOULDER_TEX], SPDR_SHOULDER_TEX_FILE, SPDR_SHOULDER_TEX_NAME, SPDR_SHOULDER_TEX);
	loadTexture(GL_TEXTURE4, textures[SPDR_UPPR_LEG_TEX], SPDR_UPPR_LEG_TEX_FILE, SPDR_UPPR_LEG_TEX_NAME, SPDR_UPPR_LEG_TEX);
	loadTexture(GL_TEXTURE5, textures[SPDR_LWR_LEG_TEX], SPDR_LWR_LEG_TEX_FILE, SPDR_LWR_LEG_TEX_NAME, SPDR_LWR_LEG_TEX);
	loadTexture(GL_TEXTURE6, textures[PLAIN_TEX], PLAIN_TEX_FILE, PLAIN_TEX_NAME, PLAIN_TEX);
	loadTexture(GL_TEXTURE7, textures[BIG_RUG_TEX], BIG_RUG_TEX_FILE, BIG_RUG_TEX_NAME, BIG_RUG_TEX);
	loadTexture(GL_TEXTURE8, textures[FLOOR_TEX], FLOOR_TEX_FILE, FLOOR_TEX_NAME, FLOOR_TEX);
	loadTexture(GL_TEXTURE9, textures[MOULDING_TEX], MOULDING_TEX_FILE, MOULDING_TEX_NAME, MOULDING_TEX);
	loadTexture(GL_TEXTURE10, textures[SML_RUG_TEX], SML_RUG_TEX_FILE, SML_RUG_TEX_NAME, SML_RUG_TEX);
	loadTexture(GL_TEXTURE11, textures[WALL_TEX], WALL_TEX_FILE, WALL_TEX_NAME, WALL_TEX);

}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	insect.keypress(key, x, y);
	switch (key) {
	case 'd':
		rotate_view_z += 5.0f;
		break;
	case 'a':
		rotate_view_z -= 5.0f;
		break;
	case 'e':
		rotate_view_x += 5.0f;
		break;
	case 'q':
		rotate_view_x -= 5.0f;
		break;
	case 'w':
		view_x -= sin(glm::radians(rotate_view_z)) * 0.3;
		view_y -= cos(glm::radians(rotate_view_z)) * 0.3;
		break;
	case 's':
		view_x += sin(glm::radians(rotate_view_z)) * 0.3;
		view_y += cos(glm::radians(rotate_view_z)) * 0.3;
		break;
		//Translate the base, etc.
	}
}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Not A Triangle");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
