#pragma once
#include <cmath>
#include <iostream>

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#  include <vector>
#pragma comment(lib, "glew32.lib") 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "hemisphere.h"

using namespace std;
using namespace glm;

//the variables to work out the accaleration



#define PI 3.14159265
#define HEM_RADIUS 2.0 // Hemisphere radius.
#define HEM_LONGS 10 // Number of longitudinal slices.
#define HEM_LATS 5 // Number of latitudinal slices.
#define HEM_COLORS 0.0, 0.0, 1.0, 1.0 // Hemisphere colors.





class Ball
{
public:
	unsigned int buffer[2];
	unsigned int vao[1];

	Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
	unsigned int hemIndices[HEM_LATS][2 * (HEM_LONGS + 1)];
	int hemCounts[HEM_LATS];
	void* hemOffsets[HEM_LATS];
	vec4 colour = vec4(1, 0, 0, 1);

	bool movement = false;
	int lasttime = 0;
	int currenttime = 0;
	float acceleration = 0.0f;
	vec3 velocity { 0.0f, 0.0f, 0.0f };
	vec3 header = vec3(0.0f, 0.0f, -1.0f) ;
	vec3 ballPosition {0.0f, 0.0f, 0.0f};
	float anglex = 0.0, angley = 0.0;

	void setup();
	void drawBall();
	void fillHemVertexArray();
	void fillHemIndices();
	void fillHemCounts();
	void fillHemOffsets();
	void collision();
	void update(float);

	// Initialize the hemisphere.
	void fillHemisphere();

	Ball();
	Ball(vec4&);
	~Ball();
};

