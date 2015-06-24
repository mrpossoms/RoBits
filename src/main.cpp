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
	// Geometry* room = new Room(Rectangle(corner[0], corner[1], 0xFF0000FF), 1);
	float t = 0;

	vec2 l1v[] = {
		{-2, 0},
		{ 1, 0},
		{ 2, 0},
		{-1, 0},		
	};
	vec2 intersect;

	ray2 r1 = {
		.p = { 0, 0 },
		.n = { 1, 0 },
	};

	ray2 r2 = {
		.p = { 1.5, 0 },
		.n = { -1, 0 },
	};

	ray2 r3 = {
		.p = { -1.5, 0 },
		.n = { 1, 0 },
	};

	p[0] = 0.5;
	p[1] = 0;

	for(int i = 1000; i--;){
		ray2 ray = {
			.p = {  },
			.n = { (RAND_F - 0.5), (RAND_F - 0.5) }
		};

		vec2_norm(ray.n, ray.n);
		vec2_scale(ray.n, ray.n, RAND_F * 9 + 1);

		float s = RAND_F * 2;
		vec2 p = { ray.n[0] * s, ray.n[1] * s };

		printf("\n%d/1000\n", 1000 - i);
		int res = vec2_point_on_ray(ray, p, &t);
		vec2_print(ray.p);
		vec2_print(ray.n);
		vec2_print(p);
		printf("vec2_point_on_ray(): t = %f s = %f\n", t, s);

		if(s > 1){
			assert(!res);
		}
		else{
			assert(res);
			assert(t <= 1 && t >= 0);
		}
	}

	int res = vec2_point_on_ray(r1, p, &t);
	printf("vec2_point_on_ray(): res = %d, t = %f\n", res, t);

	assert(res && t == 0.5);


	// -1 ======== 1
	//   (-1) <----- 1.5
	//           ^ 1
	assert(vec2_ray_line(intersect, r2, l1v[1], l1v[3], &t));
	vec2_print(intersect);
	assert(intersect[0] == 1 && intersect[1] == 0);

	assert(vec2_ray_line(intersect, r3, l1v[1], l1v[3], &t));
	vec2_print(intersect);

	assert(vec2_ray_line(intersect, r1, l1v[1], l1v[3], &t));
	vec2_print(intersect);
	// assert(intersect[0] == 0 && intersect[1] == 0);

	
	// for(int i = 0; i < 1000; ++i){
	// 	float theta = (2 * M_PI * i) / 1000.0f;
	// 	vec2 v1 = {  cos(theta),  sin(theta) };
	// 	vec2 v2 = { -cos(theta), -sin(theta) };
	// 	vec2 p = {};
	// 	float t = ((rand() % 1024) - 512) / 256.0f;

	// 	vec2_lerp(p, v1, v2, t);
	// 	int res = vec2_point_on_line(p, v1, v2);

	// 	if(t <= 1.0 && t >= 0){
	
	// 		if(!res){
	// 			printf("Test (%d/1000) failed for theta = %f\n", i, theta);
	// 			vec2_print(v1);
	// 			vec2_print(v2);
	// 			vec2_print(p);
	// 		}

	// 		assert(res);
	// 	}
	// 	else{
	// 		printf("Test (%d/1000) failed as expected for theta = %f\n", i, theta);
	// 		vec2_print(v1);
	// 		vec2_print(v2);
	// 		vec2_print(p);
	// 		assert(!res);	
	// 	}
	// }

	Line l1 = Line(l1v[0], l1v[1], 0xFF000080);
	Line l2 = Line(l1v[2], l1v[3], 0x00FF0080);

	Line lines[2];
 
	// l1.trim(l2, lines);

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

		// glLineWidth(3.0);
		// l1.draw(viewProjection);
		// l2.draw(viewProjection);

		renderer.present();
	}

	return 0;
}
