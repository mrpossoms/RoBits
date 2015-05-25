#pragma once

#include <GLFW/glfw3.h>
#include "linalg.h"

void vwrOnCursorMove(GLFWwindow* window, double x, double y);
void vwrOnClick(GLFWwindow* window, int button, int action, int mods);
void vwrOnScroll(GLFWwindow* window, double xoffset, double yoffset);

void vwrEmitViewProjection(mat4x4 vp);
