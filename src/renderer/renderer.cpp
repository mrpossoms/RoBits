#include "renderer.h"
#include <stdlib.h>
#include <stdio.h>

Renderer::Renderer(vec2 windowSize)
{
	if (!glfwInit()) exit(EXIT_FAILURE);

	window = glfwCreateWindow(
		(int)windowSize[0],
		(int)windowSize[1],
		"RoBit AI Simulator", 
		NULL,
		NULL
	);

	isRendering = 1;

	glfwMakeContextCurrent(window);
}

Renderer::~Renderer()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::stop()
{
	isRendering = 0;
}

float Renderer::aspectRatio()
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	return (float)h / (float)w;
}

int Renderer::shouldDraw()
{
	if(!isRendering) return 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return 1;
}

void Renderer::present()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}