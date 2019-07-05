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
#include <glm/gtc/type_ptr.hpp>
#include "hemisphere.h"


using namespace std;
using namespace glm;


#define CUE_RADIUS 0.7 // cue radius.
#define CUE_LONGS 15 // Number of longitudinal slices.//number of points in the circle
#define CUE_LATS 15 // Number of latitudinal slices.//number of circles
#define CUE_COLORS 0.0, 1.0, 0.0, 1.0 // Hemisphere colors.

class cue
{
public:
	unsigned int buffer[2];
	unsigned int vao[1];

	Vertex cueVertices[CUE_LATS * 2 + 2];
	unsigned int cueIndices[CUE_LATS * 2 + 2];
	int cueCounts[CUE_LATS];
	bool movement = false;
	int lasttime = 0;
	int currenttime = 0;
	float acceleration = 0.0f;
	vec3 velocity{ 0.0f, 0.0f, 0.0f };
	vec3 header = vec3(0.0f, 0.0f, -1.0f);
	vec3 cuePosition{ 0.0f, 0.0f, 0.0f };
	void* cueOffsets[CUE_LATS];

	void circle();
	void fillCueCounts();
	void fillCueOffsets();
	void drawCue();
	void setup();
	void fillCueIndices();
	void fillCue();
	void update(float);

	cue();
	~cue();
};

