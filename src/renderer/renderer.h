#pragma once

#include "linalg.h"
#include "viewer.h"

class Renderer
{
public:
	// Constructor & destructor
	Renderer(vec2 windowSize);
	~Renderer();

	void stop();

	int  shouldDraw();
	void present();

	float aspectRatio();
private:
	int isRendering;
	GLFWwindow* window;
};