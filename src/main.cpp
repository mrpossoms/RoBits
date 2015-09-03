#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <sys/time.h>
#include "renderer.h"
#include "geometry/line.h"
#include "geometry/circle.h"
#include "geometry/rectangle.h"
#include "environment/room.h"
#include "environment/robit.h"

Robit* robit;

vec4 trail[1000];
int trailIndex;
float sampleTimer;

void resetSim(GLFWwindow* win, int key, int scancode, int action, int mode)
{
	if(key == GLFW_KEY_R && robit){
		robit->reset();
	}
}

int main(int argc, char* argv[])
{
	vec2 size = { 800, 600 };
	Renderer renderer(size);

	glfwSetKeyCallback(renderer.window, resetSim);

	srand(time(NULL));
	bzero(trail, sizeof(trail));

	int fd = open("./room.bin", O_RDONLY);
	Room* room = new Room(fd);
	robit = new Robit(room, 0xDEADBEEF);

	close(fd);
	float t = 0;

	mat4x4 viewProjection;
	struct timeval lastTime;

	gettimeofday(&lastTime, NULL);
	while(renderer.shouldDraw()){
		struct timeval now;

		gettimeofday(&now, NULL);
		double n = (double)now.tv_sec + ((double)now.tv_usec * 10.0e-6);
		double l = (double)lastTime.tv_sec + ((double)lastTime.tv_usec * 10.0e-6);  
		double dt = 0.0001;//n - l;

		robit->update(dt);

		// update the view projection matrix
		vwrEmitViewProjection(viewProjection);
		mat4x4_scale_aniso(viewProjection, viewProjection, renderer.aspectRatio(), 1, 1);

		// rendering and update logic
		room->draw(viewProjection);
		robit->draw(viewProjection);

		if(sampleTimer <= 0){
			sampleTimer = 1;
			trail[trailIndex][3] = 1;
			vec2_set(trail[trailIndex++], robit->position);
			trailIndex = trailIndex % 1000;
		}
		else{
			sampleTimer -= dt;
		}

		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < 1000; ++i){
			int ti = mod(trailIndex - i, 1000);
			vec4 temp;

			if(trail[ti][3] == 1){
				mat4x4_mul_vec4(temp, viewProjection, trail[ti]);
				glVertex2f(temp[0], temp[1]);
				glColor4f(0, (1000 - i) / 1000.0f, 0, 1.0);
			}
		}
		glEnd();

		renderer.present();
		lastTime = now;
	}

	return 0;
}
