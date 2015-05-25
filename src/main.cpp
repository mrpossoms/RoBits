#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "geometry/line.h"
#include "geometry/circle.h"
#include "geometry/rectangle.h"
#include "environment/room.h"

int main(int argc, char* argv[])
{
	vec2 size = { 800, 600 };
	vec2 v1 = { -1, -1 }, v2 = { 1, 1 };
	vec2 p = {};
	Renderer renderer(size);

	srand(time(NULL));

	vec2 corner[] = {
		{ -2, -2 },
		{  2,  2 }
	};
	Geometry* room = new Room(Rectangle(corner[0], corner[1], 0xFF0000FF), 10);

	mat4x4 viewProjection;
	float t = 0;

	while(renderer.shouldDraw()){

		// update the view projection matrix
		vwrEmitViewProjection(viewProjection);
		mat4x4_scale_aniso(viewProjection, viewProjection, renderer.aspectRatio(), 1, 1);

		// vec4 pos = { cos(t += 0.01f), 0, 0, 1 };
		// vec4_set(((Circle*)cir)->position, pos);

		// vec2 norm = {};
		// if(cir->intersects(geo, norm)){
		// 	cir->setColor(0x00FF00FF);
		// }
		// else{
		// 	cir->setColor(0xFF0000FF);
		// }

		// rendering and update logic
		room->draw(viewProjection);

		renderer.present();
	}

	return 0;
}
