#define _USE_MATH_DEFINES
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

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
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/decomposition.hpp>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "textures.h"
#include "maths_funcs.h"
#include "insect.h"
#include "house.h"
#include "lantern.h"
#include "spoids.h"

#define MESH_NAME "Models/insect.dae"



using namespace std;
GLuint shaderProgramID;

//mat4 biasMatrix;
//mat4 textureMatrix;

//Insect insect = Insect();
Spoids insects = Spoids();
House house = House();
Lantern lantern;
Model cube;
unsigned int mesh_vao = 0;
int width = 800;
int height = 600;

GLfloat rotate_y = 0.0f;
GLfloat rotate_view_x = -90.0f, rotate_view_z = 0.0f;
GLfloat view_x = -5.0f, view_y = 15.0f;

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
	int light_position_location = glGetUniformLocation(shaderProgramID, "lightPosition");
	int light_location = glGetUniformLocation(shaderProgramID, "light");
	int depthMatrixID = glGetUniformLocation(shaderProgramID, "depthMatrix");


	// Root of the Hierarchy
	mat4 view = identity_mat4();
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	mat4 model = identity_mat4();
	mat4 groundTransformation = identity_mat4();
	mat4 houseTransformation = identity_mat4();
	mat4 lanternTransformation = identity_mat4();
	//model = translate(model, vec3(3.0f, 0.0f, 0.7f));
	//model = rotate_z_deg(model, -rotate_y);

	//shadows????
	//glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);

	//// Compute the MVP matrix from the light's point of view
	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	//glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//glm::mat4 depthModelMatrix = glm::mat4(1.0);
	//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	//// Send our transformation to the currently bound shader,
	//// in the "MVP" uniform
	//glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
	//end shadows


	view = translate(view, vec3(view_x, view_y, -5));
	lanternTransformation = rotate_x_deg(lanternTransformation, 90);

	lanternTransformation = scale(lanternTransformation, vec3(0.35f, 0.35f, 0.35f));
	lanternTransformation = translate(lanternTransformation, vec3(-0.6f, 2.0f, 0.0f));
	lanternTransformation = rotate_z_deg(lanternTransformation, -rotate_view_z);

	lanternTransformation = translate(lanternTransformation, vec3(-view_x, -view_y, 4));
	//doorTransformation = translate(doorTransformation, vec3(view_x, view_y, 0));
	//lanternTransformation = translate(lanternTransformation, vec3(5, -5, -5));
	view = rotate_z_deg(view, rotate_view_z);
	view = rotate_x_deg(view, rotate_view_x);

	vec3 lightPos = vec3(lanternTransformation.m[12], lanternTransformation.m[13], lanternTransformation.m[14]);

	mat4 lightTransform = identity_mat4();
	lightTransform = translate(lightTransform, lightPos);
	//vec3 lightPos = vec3(-view_x, -view_y, 4) + vec3(-0.6f, 2.0f, 0.0f);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glUniform3fv(light_position_location, 1, lightPos.v);
	glUniform1i(light_location, false);
	//glBindTexture(GL_TEXTURE_2D, textures[SPDR_BODY_TEX]);
	insects.draw(model, matrix_location, texture_number_loc);
	//cube.draw(lightTransform, identity_mat4(), matrix_location, texture_number_loc);
	house.draw(identity_mat4(), matrix_location, texture_number_loc);
	//ground.draw(groundTransformation, identity_mat4(), matrix_location, textures[SPDR_BODY_TEX]);
	//door.draw(doorTransformation, identity_mat4(), matrix_location, textures[SPDR_BODY_TEX]);
	//lantern.draw(lanternTransformation, identity_mat4(), matrix_location, texture_number_loc);
	lantern.draw(lanternTransformation, matrix_location, texture_number_loc, light_location);

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

	insects.update(delta);

	// Draw the next frame
	glutPostRedisplay();
}

void setupShadows() {
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("shadows not working");
		//return false;
}

void init()
{
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();

	insects = Spoids(MESH_NAME);
	house = House("Models/house_full.obj");
	//house = Model(Model::loadScene("Models/house_full.obj"));
	lantern = Lantern("Models/lantern.obj");
	cube = Model(Model::loadScene("Models/cube.obj"), FIRE_TEX);

	insects.generateObjectBufferMesh(shaderProgramID);
	house.generateObjectBufferMesh(shaderProgramID);
	lantern.generateObjectBufferMesh(shaderProgramID);
	cube.generateObjectBufferMesh(shaderProgramID);

	loadAllTextures(shaderProgramID);


	//Calculate texture matrix for projection
////This matrix takes us from eye space to the light's clip space
////It is postmultiplied by the inverse of the current view matrix when specifying texgen
//
//	biasMatrix = mat4(0.5f, 0.0f, 0.0f, 0.0f,
//		0.0f, 0.5f, 0.0f, 0.0f,
//		0.0f, 0.0f, 0.5f, 0.0f,
//		0.5f, 0.5f, 0.5f, 1.0f);
//
//	textureMatrix = biasMatrix * lightProjectionMatrix*lightViewMatrix;
//
//	//Set up texture coordinate generation.
//	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
//	glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(0));
//	glEnable(GL_TEXTURE_GEN_S);
//
//	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
//	glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.GetRow(1));
//	glEnable(GL_TEXTURE_GEN_T);
//
//	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
//	glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.GetRow(2));
//	glEnable(GL_TEXTURE_GEN_R);
//
//	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
//	glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.GetRow(3));
//	glEnable(GL_TEXTURE_GEN_Q);
//
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	//insects.keypress(key, x, y);
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
		view_x -= sin(glm::radians(rotate_view_z)) * 0.3f;
		view_y -= cos(glm::radians(rotate_view_z)) * 0.3f;
		break;
	case 's':
		view_x += sin(glm::radians(rotate_view_z)) * 0.3f;
		view_y += cos(glm::radians(rotate_view_z)) * 0.3f;
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
