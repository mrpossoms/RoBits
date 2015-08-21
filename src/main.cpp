#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <GLFW/glfw3.h>
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

	Line l1(0, 0, 1, 0, 0xFF0000FF);
	Line l2(-1, 0.25, 1, 0.25, 0x00FF00FF);

	int fd = open("./room.bin", O_RDONLY);
	Room* room = new Room(fd);
	Robit* robit = new Robit(room);

	close(fd);
	float t = 0;

	mat4x4 viewProjection;

	while(renderer.shouldDraw()){

		// update the view projection matrix
		vwrEmitViewProjection(viewProjection);
		mat4x4_scale_aniso(viewProjection, viewProjection, renderer.aspectRatio(), 1, 1);

		// rendering and update logic
		room->draw(viewProjection);

		l1.vertices[1][0] = cos(t);
		l1.vertices[1][1] = sin(t);
		t += 0.01;

		vec2 inter;
		float interTime = -1;
		ray2 r;

		vec2_set(r.p, l1.vertices[0]);
		vec2_set(r.n, l1.vertices[1]);

		if(vec2_line_line(inter, l1.vertices[0], l1.vertices[1], l2.vertices[0], l2.vertices[1], &interTime)){
			l1.setColor(0xFFFFFFFF);
		}
		else{
			l1.setColor(0xFF0000FF);
		}

		glBegin(GL_LINES);
		l1.draw(viewProjection);
		l2.draw(viewProjection);
		glEnd();

		robit->draw(viewProjection);

		renderer.present();
	}

	return 0;
}
