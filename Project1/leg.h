#ifndef LEG_H
#define LEG_H

#include "model.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

class Leg {
public:

	Leg(Mesh upprLeg, Mesh lwrLeg);
	Leg();

	void generateObjectBufferMesh(GLuint shaderProgramID);

	void draw(mat4 parent, GLuint matrix_location, int texture_number_loc);

	void update(float delta);

	void keypress(unsigned char key, int x, int y);

	void alternateMotion();
		
	void switchSide();

private:
	int upperLegTex = 1;
	int lowerLegTex = 5;
	Mesh upperLeg;
	Mesh lowerLeg;
	GLfloat motion_y = 0.6f;
	GLfloat motion_x = 0.6f;
	GLfloat cycleLength = 20.0f;
	GLfloat cyclePos_x = -cycleLength;
	GLfloat cyclePos_y = 0;
	GLfloat hip_transform_x = -0.1f;
	GLfloat hip_transform_z = 0.45f;
	GLfloat knee_transform_x = 0.9f;
	GLfloat knee_transform_z = -0.0075f;
	int rotate_dir = 1;
};

#endif // !LEG_H
