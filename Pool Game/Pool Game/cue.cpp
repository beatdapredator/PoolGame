#include "cue.h"



cue::cue()
{
	fillCue();
}


cue::~cue()
{
}

void cue::update(float deltatime)
{
	if (movement == true) //I have a bool value set to false initially then when space key is pressed will run the code below.
	{
		velocity = velocity + (header*acceleration);// the equation to work out velocity
		cuePosition = cuePosition + (velocity * deltatime);
	}
}

void cue::circle()
{
	int k = 0;
	float amount = 360.0f / CUE_LATS; //use this to woek out the angle between points
	for (float j = 0; j <= 360.0f; j+= amount) //for the float of j, if j < or equal to 360 then do j plus the new angle
	{ // radiance is worked out using pi rather than degrees
		cueVertices[k].coords.x = CUE_RADIUS * cos(radians(j)); //to work out the first coordinate you need to do the radius of the circle * the cosine of the radiance of j
		cueVertices[k].coords.y = CUE_RADIUS * sin(radians(j)); //to work out the second coordinate you need to do the radius of the circle * the sine of the radiance of j
		cueVertices[k].coords.z = 0; // the z cooridinate = 0
		cueVertices[k].coords.w = 1.0;
		cueVertices[k].colors = vec4(0.97, 0.9, 0.65, 1);
		k++;
	}

	for (float j = 0; j <= 360.0f; j += amount )
	{
		cueVertices[k].coords.x = CUE_RADIUS * cos(radians(j));//to work out the first coordinate you need to do the radius of the circle * the cosine of the radiance of j
		cueVertices[k].coords.y = CUE_RADIUS * sin(radians(j));//to work out the second coordinate you need to do the radius of the circle * the sine of the radiance of j
		cueVertices[k].coords.z = 60;// the z cooridinate is equal to 10 times the first one so that it is 10 units further away.
		cueVertices[k].coords.w = 1.0;
		cueVertices[k].colors = vec4(0.0, 0.0, 0.0, 1);
		k++;
	}
}
void cue::fillCueIndices()
{
	int i, j;
	i = 0;
	for (j = 0; j < CUE_LATS; j++)
	{
		cueIndices[i] = j;
		i++;
		cueIndices[i] = j + CUE_LATS;
		i++;
	}
	cueIndices[i] = 0;
	i++;
	cueIndices[i] = CUE_LATS;

	for (int i = 0; i < CUE_LATS * 2 + 2; i++)
	{
		cout << cueIndices[i] << endl;
	}


}

void cue::setup()
{
	glGenVertexArrays(1, vao);
	glGenBuffers(2, buffer);
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cueVertices), cueVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cueIndices), cueIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(cueVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(cueVertices[0]), (GLvoid*)sizeof(cueVertices[0].coords));
	glEnableVertexAttribArray(1);
}

// Fill the array of counts.
void cue::fillCueCounts()
{
	int j;
	for (j = 0; j < CUE_LATS; j++) cueCounts[j] = 2 * (CUE_LONGS + 1);
}

// Fill the array of buffer offsets.
void cue::fillCueOffsets()
{
	int j;
	for (j = 0; j < CUE_LATS; j++) cueOffsets[j] = (GLvoid*)(2 * (CUE_LONGS + 1)*j * sizeof(unsigned int));
}

void cue::drawCue()
{
	glBindVertexArray(vao[0]);
	glDrawElements(GL_TRIANGLE_STRIP, (CUE_LATS * 2 + 2), GL_UNSIGNED_INT, 0);

}
// Initialize the Cue
void cue::fillCue()
{
	circle();
	fillCueIndices();
}