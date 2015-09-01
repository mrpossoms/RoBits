#include "viewer.h"
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

static struct utsname VWR_OS;

vec2 VWR_POSITION;
float VWR_ZOOM = 0.5;
vec2 VWR_CURSOR_POS, VWR_LAST_CURSOR_POS;
int  VWR_LEFT_DOWN;

void vwrOnCursorMove(GLFWwindow* window, double x, double y)
{
	if(!VWR_OS.sysname[0]){
		uname(&VWR_OS);
	}

	vec2_set(VWR_LAST_CURSOR_POS, VWR_CURSOR_POS);

	VWR_CURSOR_POS[0] = -x / 300.0f;
	VWR_CURSOR_POS[1] =  y / 300.0f;

	if(VWR_LEFT_DOWN){
		vec2 delta;
		vec2_sub(delta, VWR_LAST_CURSOR_POS, VWR_CURSOR_POS);

		if(!strcmp("Linux", VWR_OS.sysname)){
			vec2_scale(delta, delta, 1.5f);
			vec2_add(VWR_POSITION, VWR_POSITION, delta);
		}
		else{
			VWR_ZOOM += delta[1];
			if(VWR_ZOOM < 0.25f) VWR_ZOOM = 0.25f;
		}

	}
}

void vwrOnClick(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS){
		VWR_LEFT_DOWN = 1;
	}
	else{
		VWR_LEFT_DOWN = 0;
	}
}

void vwrOnScroll(GLFWwindow* window, double xoffset, double yoffset)
{

	if(!strcmp("Linux", VWR_OS.sysname)){
		VWR_ZOOM += (float)yoffset / 10.0f;
		if(VWR_ZOOM < 0.25f) VWR_ZOOM = 0.25f;
	}
	else{
		vec2 delta = { (float)xoffset / 10.0f, -(float)yoffset / 10.0f };
		vec2_add(VWR_POSITION, VWR_POSITION, delta);
	}
}

void vwrEmitViewProjection(mat4x4 vp)
{
	mat4x4_translate(vp, VWR_POSITION[0], VWR_POSITION[1], 0);
	mat4x4_scale(vp, vp, VWR_ZOOM);

}
