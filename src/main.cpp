#include <unistd.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "geometry/line.h"
#include "geometry/circle.h"

int main(int argc, char* argv[])
{
	vec2 size = { 800, 600 };
	vec2 v1 = { -1, -1 }, v2 = { 1, 1 };
	vec2 p = {};
	Renderer renderer(size);
	Geometry* geo = new Line(v1, v2, 0xFF0000FF);
	Geometry* cir = new Circle(p, 0.25f, 0x00FFFFFF);

	mat4x4 viewProjection;
	float t = 0;

	while(renderer.shouldDraw()){

		// update the view projection matrix
		mat4x4_identity(viewProjection);
		mat4x4_scale_aniso(viewProjection, viewProjection, renderer.aspectRatio(), 1, 1);

		vec4 pos = { cos(t += 0.01f), 0, 0, 1 };
		vec4_set(((Circle*)cir)->position, pos);

		// rendering and update logic
		geo->draw(viewProjection);
		cir->draw(viewProjection);

		renderer.present();
	}

	return 0;
}