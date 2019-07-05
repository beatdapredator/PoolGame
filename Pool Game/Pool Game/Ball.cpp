#include "Ball.h"


Ball::Ball()
{
	fillHemisphere();
}

Ball::Ball(vec4& _colour)
{
	colour = _colour;
	fillHemisphere();
}


Ball::~Ball()
{
}

void Ball::collision()//This is my function within my ball that will allow me to collide with the walls.
{
	if (ballPosition.z < -96)//For each section of the edge of my pool table I see if the ball goes further than the edge of table 
		velocity.z *= -1;//If it does using a simple if statement it will make the velocity on this axis -1 reversing the direction it is going.

	if (ballPosition.z > 37)
		velocity.z *= -1;

	if (ballPosition.x < -35)
		velocity.x *= -1;

	if (ballPosition.x > 35)
		velocity.x *= -1;
	//This is a very simple and naive way of making this work and for other projects may not work the way that I have shown in the video.
}
// Fill the vertex array with co-ordinates of the sample points.
void Ball::fillHemVertexArray()
{
	int i, j, k;

	k = 0;
	for (j = 0; j <= HEM_LATS; j++)
		for (i = 0; i <= HEM_LONGS; i++)
		{
			hemVertices[k].coords.x = HEM_RADIUS * cos((float)j / HEM_LATS * PI / 2.0) * cos(2.0 * (float)i / HEM_LONGS * PI);
			hemVertices[k].coords.y = HEM_RADIUS * sin((float)j / HEM_LATS * PI / 2.0);
			hemVertices[k].coords.z = HEM_RADIUS * cos((float)j / HEM_LATS * PI / 2.0) * sin(2.0 * (float)i / HEM_LONGS * PI);
			hemVertices[k].coords.w = 1.0;
			hemVertices[k].colors = colour;
			k++;
		}
}

// Fill the array of index arrays.
void Ball::fillHemIndices()
{
	int i, j;
	for (j = 0; j < HEM_LATS; j++)
	{
		for (i = 0; i <= HEM_LONGS; i++)
		{
			hemIndices[j][2 * i] = (j + 1)*(HEM_LONGS + 1) + i;
			hemIndices[j][2 * i + 1] = j*(HEM_LONGS + 1) + i;
		}
	}
}

void Ball::setup()
{
	glGenVertexArrays(1, vao);
	glGenBuffers(2, buffer);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hemVertices), hemVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hemIndices), hemIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(hemVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(hemVertices[0]), (GLvoid*)sizeof(hemVertices[0].coords));
	glEnableVertexAttribArray(1);
}

void Ball::update(float deltatime)
{
	if (movement == true) //I have a bool value set to false initially then when space key is pressed will run the code below.
	{
		velocity = velocity + (header*acceleration);// the equation to work out velocity
		ballPosition = ballPosition + (velocity * deltatime);//This section is used to update the balls position dependent on the velosity and deltatime added to the old position

		acceleration = 0.0f; // I set the acceleration to 0
		velocity *= 0.999f; // the velocity is the drag on the ball and I have that set quite high as the ball did not move
		float Length = length(velocity); // I serch for the length of the vector of my velocity so that I can use that to reset my movement
		angley = 0.0;// this is the angle that I will rotate my cue ball on
	

		if (Length < 0.0001) //if the lenght of my velocity is less than 0.0001
		{
			movement = false;//the movement bool is reset to 0
			cout << "movement " << movement << endl; // this chechs whether the boolean has been reset and prints a 0 in the console
			velocity = vec3(0, 0, 0);// this finally resets the velocity vector to 0,0,0
		}
	}
}
// Fill the array of counts.
void Ball::fillHemCounts()
{
	int j;
	for (j = 0; j < HEM_LATS; j++) hemCounts[j] = 2 * (HEM_LONGS + 1);
}

// Fill the array of buffer offsets.
void Ball::fillHemOffsets()
{
	int j;
	for (j = 0; j < HEM_LATS; j++) hemOffsets[j] = (GLvoid*)(2 * (HEM_LONGS + 1)*j * sizeof(unsigned int));
}

void Ball::drawBall()
{
	glBindVertexArray(vao[0]);
	glMultiDrawElements(GL_TRIANGLE_STRIP, hemCounts, GL_UNSIGNED_INT, (const void **)hemOffsets, HEM_LATS);

}
// Initialize the hemisphere.
void Ball::fillHemisphere()
{
	fillHemVertexArray();
	fillHemIndices();
	fillHemCounts();
	fillHemOffsets();
}