#pragma once
struct vec3 {
	typedef const vec3& i;
	typedef vec3& io;

	float x, y, z;
	//konstruktory
	vec3() {};
	vec3(float x2, float y2, float z2) : x(x2), y(y2), z(z2) {};
	vec3(float x2, float y2, float z2, float len) :x(x2), y(y2), z(z2) {};
};
struct primitives {
	vec3 location;
};
struct triangle :primitives
{
	vec3 * wierzcholki = new vec3[3];
	triangle(vec3 wierzcholki2[3]) {}
};
