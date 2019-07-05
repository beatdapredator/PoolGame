#include <cmath>
#include <iostream>

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#  include <vector>
#  include <glm/gtx/rotate_vector.hpp>
#pragma comment(lib, "glew32.lib") 


#include "Ball.h"	
#include "Cue.h"

using namespace std;
using namespace glm;

int lasttime = 0;
int currenttime = 0;
#define VERTICES 0
#define INDICES 1
#define ANNULUS 0
#define TRIANGLE 1

Ball ball1;
Ball ball2(vec4(0.9, 0.6, 0.01, 1));
Ball ball3(vec4(0.9, 0.6, 0.01, 1));
Ball ball4(vec4(0.9, 0.6, 0.01, 1));
Ball ball5;
Ball ball6;
Ball ball7;
Ball ball8(vec4(0.9, 0.6, 0.01, 1));
Ball ball9;
Ball ball10;
Ball ball11(vec4(0.0, 0.0, 0.0, 1));
Ball ball12(vec4(0.9, 0.6, 0.01, 1));
Ball ball13(vec4(0.9, 0.6, 0.01, 1));
Ball ball14;
Ball ball15;
Ball ballwhite(vec4(1.0, 1.0, 1.0, 1));
cue testcue;

struct Matrix4x4
{
	float entries[16];
};

static const Matrix4x4 IDENTITY_MATRIX4x4 =
{
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	}
};

static enum buffer { SQUARE_VERTICES };
static enum object { SQUARE };

static Vertex Table[] =
{
	{ { -40.0, 0.0, 50.0, 1.0 },{ 0.17, 0.75, 0.12, 1.0 } },
	{ { -40.0, 0.0, -80.0, 1.0 },{ 0.17, 0.75, 0.12, 1.0 } },
	{ { 40.0, 0.0, 50.0, 1.0 },{ 0.17, 0.75, 0.12, 1.0 } },
	{ { 40.0, 0.0, -80.0, 1.0 },{ 0.17, 0.75, 0.12, 1.0 } }
};

static Vertex Barrier[] =
{
	{ { -40.0, 0.0, 50.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { -40.0, 5.0, 50.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 40.0, 0.0, 50.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 40.0, 5.0, 50.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 40.0, 0.0, -80.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { 40.0, 5.0, -80.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { -40.0, 0.0, -80.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { -40.0, 5.0, -80.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { -40.0, 0.0, 50.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } },
	{ { -40.0, 5.0, 50.0, 1.0 },{ 0.0, 0.0, 0.0, 1.0 } }
};

mat4 modelViewMat, projMat;

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc;

// Triangle strip vertex indices in order.
static unsigned int buffer[2]; // Array of buffer ids.

static unsigned int vao[2]; // Array of VAO ids.
							// End globals.

void shaderCompileTest(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}

// Function to read text file.
char* readTextFile(char* aTextFile)
{
	FILE* filePointer = fopen(aTextFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Create shader program executable.
	char* vertexShader = readTextFile("vertexShader.glsl");
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);
	shaderCompileTest(vertexShaderId);

	char* fragmentShader = readTextFile("fragmentShader.glsl");
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);
	shaderCompileTest(fragmentShaderId);

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);




	// Create VAO and VBO and associate data with vertex shader.
	glGenVertexArrays(2, vao);
	glGenBuffers(2, buffer);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Table), Table, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Table[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Table[0]), (GLvoid*)sizeof(Table[0].coords));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Barrier), Barrier, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Barrier[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Barrier[0]), (GLvoid*)sizeof(Barrier[0].coords));
	glEnableVertexAttribArray(1);


	///////////////////////////////////////

	// Obtain modelview matrix uniform location and set value.

	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	//glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	///////////////////////////////////////

	ball1.setup();
	ball2.setup();
	ball3.setup();
	ball4.setup();
	ball5.setup();
	ball6.setup();
	ball7.setup();
	ball8.setup();
	ball9.setup();
	ball10.setup();
	ball11 .setup();
	ball12.setup();
	ball13.setup();
	ball14.setup();
	ball15.setup();
	ballwhite.setup();
	testcue.setup();
}

// Drawing routine.
void drawScene(void)
{
	ballwhite.collision();//calls the cue balls collision for each frame

	float deltatime = 0;
	currenttime = glutGet(GLUT_ELAPSED_TIME);
	deltatime = (currenttime - lasttime) / 1000.0f;  // }this is how i worked out delta time for the game.
	lasttime = currenttime;
	ballwhite.update(deltatime);

	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100,-40), vec3(0,0,0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(0.0, 0, 15.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	testcue.drawCue();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -60.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball1.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball1.drawBall();
	
	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(-5.0, 0, -60.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball2.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball2.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(5.0, 0, -60.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball3.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball3.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(10.0, 0, -60.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball4.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball4.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(-10.0, 0, -60.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball5.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball5.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(-7.0, 0, -55.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball6.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball6.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(-2.5, 0, -55.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball7.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball7.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(2.5, 0, -55.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball8.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball8.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(7.0, 0, -55.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball9.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball9.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(-5.0, 0, -50.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball10.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball10.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(0.0, 0, -50.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball11.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball11.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(5.0, 0, -50.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball12.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball12.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(-2.5, 0, -45.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball13.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball13.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(2.5, 0, -45.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball14.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball14.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(0.0, 0, -40.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball15.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ball15.drawBall();

	modelViewMat = mat4(1.0f);
	modelViewMat = lookAt(vec3(0, 100, -40), vec3(0, 0, 0), vec3(0, 1, 0));
	modelViewMat = translate(modelViewMat, vec3(0.0, 0, 15.0));
	modelViewMat = translate(modelViewMat, ballwhite.ballPosition);
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ballwhite.drawBall();
	modelViewMat = scale(modelViewMat, vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	ballwhite.drawBall();
	glutPostRedisplay();
	glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	/////////////////////////////////////////perspective view//////////////////////////////////////////////////////////
	projMat = perspective(radians(90.0f), (float)(w)/(float)(h), 0.01f, 1000.0f);//frustum(-100.0f, 100.0f, -100.0f, 100.0f, 0.01f, 200.0f);
	projMatLoc = glGetUniformLocation(programId, "projMat");
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	
		switch (key)
		{
		case 27:
			exit(0);
			break;
		case 'w':
			ballwhite.acceleration += 10.0f;//When w is pressed it increses the acceleration by 10 each frame
			cout << "cueball acceleleration" << ballwhite.acceleration << endl;
			glutPostRedisplay();
			break;
		case 's':
			ballwhite.acceleration -= 10.0f;//When s is pressed it reduces the acceleration by 10 each frame
			cout << "cueball acceleleration" << ballwhite.acceleration << endl;
			glutPostRedisplay();
			break;
		case 'a':
			ballwhite.angley += 0.7;//when you press a or d you can rotate the ball around the y axis ny 0.7 each frame
			cout << "ball direction" << ballwhite.angley << endl;
			ballwhite.header = normalize(rotateY(ballwhite.header, radians(0.7f)));
			glutPostRedisplay();
			break;
		case 'd':
			ballwhite.angley -= 0.7;
			cout << "ball direction" << ballwhite.angley << endl;
			ballwhite.header = normalize(rotateY(ballwhite.header, radians(0.7f)));
			glutPostRedisplay();
			break;
		case ' ':
			ballwhite.movement = true;//By using the space key it changes the boolean value of the movement variable to true which will then run my movement code to move the ball by acceleration
			cout << "shoot" << endl;
			glutPostRedisplay();
			break;
		default:
			break;
		}
	
}
void keySpecialInput(int key, int x, int y)
{
	switch (key)
	{

	case GLUT_KEY_UP:
		
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
	
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:

		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:

		glutPostRedisplay();
		break;
	default:
		break;
	}
}
// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("PoolGame");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(keySpecialInput);

	glewExperimental = GL_TRUE;
	glewInit();
	cout << "Made by Matthew Dunphy" << endl;
	setup();

	glutMainLoop();
}

