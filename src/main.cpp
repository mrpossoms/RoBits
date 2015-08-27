#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <GLFW/glfw3.h>
#include <sys/time.h>
#include "renderer.h"
#include "geometry/line.h"
#include "geometry/circle.h"
#include "geometry/rectangle.h"
#include "environment/room.h"
#include "environment/robit.h"

int main(int argc, char* argv[])
{
	vec2 size = { 800, 600 };
	Renderer renderer(size);

	srand(time(NULL));

	int fd = open("./room.bin", O_RDONLY);
	Room* room = new Room(fd);
	Robit* robit = new Robit(room, 0xDEADBEEF);

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
		double dt = 0.1;//n - l;

		robit->update(dt);

		// update the view projection matrix
		vwrEmitViewProjection(viewProjection);
		mat4x4_scale_aniso(viewProjection, viewProjection, renderer.aspectRatio(), 1, 1);

		// rendering and update logic
		room->draw(viewProjection);
		robit->draw(viewProjection);

		renderer.present();
		lastTime = now;
	}

	return 0;
}
