#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
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
	// Geometry* room = new Room(Rectangle(corner[0], corner[1], 0xFF0000FF), 10);
	float t = 0;

	vec2 l1v[] = {
		{-2, 0},
		{ 1, 0},
		{ 2, 0},
		{-1, 0},		
	};
	// vec2 intersect;

	// ray2 r1 = {
	// 	.p = { 0, 0 },
	// 	.n = { 1, 0 },
	// };

	// ray2 r2 = {
	// 	.p = { 1.5, 0 },
	// 	.n = { -1, 0 },
	// };

	// ray2 r3 = {
	// 	.p = { -1.5, 0 },
	// 	.n = { 1, 0 },
	// };

	// assert(vec2_ray_line2(intersect, r2, l1v[1], l1v[3], &t));
	// assert(vec2_ray_line2(intersect, r3, l1v[1], l1v[3], &t));

	// assert(vec2_ray_line2(intersect, r1, l1v[1], l1v[3], &t));
	// assert(intersect[0] == 0 && intersect[1] == 0);

	// vec2_print(intersect);


	Line l1 = Line(l1v[0], l1v[1], 0xFF000080);
	Line l2 = Line(l1v[2], l1v[3], 0x00FF0080);

	Line lines[2];
 
	 l1.trim(l2, lines);

	l1.vertices[0][1] += 0.1;
	l1.vertices[1][1] += 0.1;
	l2.vertices[0][1] += 0.1;
	l2.vertices[1][1] += 0.1;

	mat4x4 viewProjection;

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
		// room->draw(viewProjection);

		glLineWidth(3.0);
		l1.draw(viewProjection);
		l2.draw(viewProjection);

		glLineWidth(1.0);
		for(int i = 2; i--;){
			lines[i].draw(viewProjection);
		}

		renderer.present();
	}

	return 0;
}
