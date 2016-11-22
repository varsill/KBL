#include <cstdlib>
#include <limits>
#include <math.h>
#include <vector>
#include <string>
using namespace std;
std::numeric_limits<float> real;
const float eps3 = 0.001f;
const float eps5 = 0.00001f;
const float inf = real.infinity();
const float undefined = real.infinity();
const float Pi = 3.14159265f;
const float PiMul2 = 2 * Pi;
typedef unsigned int nat;
#define nt(x) ((nat)(x))
#define fl(x) ((float)(x))
#define bt(x) ((unsigned char )(x))
#define isNull(ptr) (NULL==ptr)
#define notNull(ptr) (NULL != ptr)
#define CUT(x) ( x < 0.0f ? 0.0f : x ) //przycinanie wartosci mniejszych od zera

inline float uRand() { return (fl(rand()) / fl(RAND_MAX)); }

struct vec2 {
	typedef const vec2& i; // input ref
	float u, v;

	vec2(float u, float v) : u(u), v(v) {}
	vec2() { u = v = 0.0f; }
}; 
// długość wektora 2d
inline float length(vec2::i w)
{
	return sqrt(w.u * w.u + w.v * w.v);
}

///// operacje arytmetyczne dla wektora 2d

inline vec2 operator+ (vec2::i a, vec2::i b)
{
	return vec2(a.u + b.u, a.v + b.v);
}

inline vec2 operator- (vec2::i a, vec2::i b)
{
	return vec2(a.u - b.u, a.v - b.v);
}

inline vec2 operator* (vec2::i a, float t)
{
	return vec2(a.u * t, a.v * t);
}

struct vec3 {
	typedef const vec3& i;
	typedef vec3& io;

	float x, y, z;
	//konstruktory
	vec3() { x = y = z = 0.0f; }
	vec3(float x2, float y2, float z2) : x(x2), y(y2), z(z2){}
	vec3(float x2, float y2, float z2, float len) :x(x2), y(y2), z(z2)
	{
		float newLen = len / sqrt(x*x + y*y + z*z);
		x *= newLen; y *= newLen; z *= newLen;
	}
	
};
	//OPERACJE
	//dodawanie
	inline  vec3 operator + (vec3::i a, vec3::i b)
	{
		return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	//odejmowanie
	inline  vec3  operator - (vec3::i a, vec3::i b)
	{
		return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
	}
	//mnozenie wektorow przez liczbe
	inline vec3  operator * (vec3::i a, float t)
	{
		return vec3(a.x*t, a.y*t, a.z*t);
	}
	//dzielenie wektorow przez liczbe
	inline vec3  operator / (vec3::i a, float t)
	{
			return vec3(a.x/t, a.y/t, a.z/t);
	}
	//odwracaie wektora
	inline vec3 invert(vec3::i a)
	{
		return vec3(-a.x, -a.y, -a.z);
	}
	//przeksztalcanie na wektor jednostkowy
	inline vec3 unitise(vec3::i a)
	{
		return vec3(a.x, a.y, a.z, 1.0f);
	}
	//zwracanie dlugosci wektora
	inline float length(vec3::i a)
	{
		return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	}
	//zwracanie odleglosci pomiedzy punktami
	inline float distance(vec3::i p1, vec3::i p2)
	{
		return length(p2- p1);
	}
	//iloczyn skalarny
	inline float dotProd(vec3::i a, vec3::i b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	//iloczyn wektorowy
	inline vec3 uCrossProd(vec3::i a, vec3::i b)
	{
		return vec3(
			a.y + b.z - b.y*a.z,
			a.z*b.x - b.z*a.x,
			a.x*b.y - b.x*a.y,
			1.0f);
	}
	//przeniesienie wektora V do przestrzeni zbudowanej na wektorach Ex, Ey, Ez oraz jego normalizacja
	inline vec3 uChangeSpace(vec3::i V, vec3::i Ex, vec3::i Ey, vec3::i Ez) {
		return vec3(
			V.x*Ex.x + V.y*Ey.x + V.z*Ez.x,
			V.x*Ex.y + V.y*Ey.y + V.z*Ez.y,
			V.x*Ex.z + V.y*Ey.z + V.z*Ez.z,
			1.0f
			);
	}


//PROMIEN SWIATLA
struct ray {
	typedef const ray& i;
	vec3 origin;
	vec3 abackDir;
	vec3 aheadDir;
	ray(vec3 origin, vec3 aheadDir2) :origin(origin), aheadDir(aheadDir2) {};

};
//KOLOR
struct color {
	typedef const color& i;
	typedef color& io;
	float r, b, g;
	color() { r = b = g = 0.0f; }
	color(float r2, float g2, float b2): r(r2), g(g2), b(b2){}
	color(float v) { r = g = b = CUT(v); }
};
//definicje kolorow
color black(0.0f),
grey(0.9f),
white(1.0f),
red(0.95f, 0.32f, 0.23f),
green(0.71f, 0.89f, 0.1f),
blue(0.19f, 0.77f, 0.98f),
darkBlue(0.04f, 0.42f, 0.5f),
yellow(1.0f, 0.98f, 0.74f);
//OPERACJE NA KOLORACH
//czy kolor jest czarny?
inline bool isBlack(color::i c)
{
	return (c.r + c.g + c.b > 0.0f) ? false : true;
}
//dodawanie kolorow
inline color  operator + (color::i a, color::i b)
{
	return color(a.r + b.r, a.g + b.g, a.b + b.b);
}
inline color  operator - (color::i a, color::i b)
{
	return color(a.r - b.r, a.g - b.g, a.b - b.b);
}
inline color  operator * (color::i a, color::i b)
{
	return color(a.r * b.r, a.g  * b.g, a.b * b.b);
}
inline color  operator * (color::i a, float t)
{
	return color(a.r*t, a.g*t, a.b*t);
}
inline color  operator / (color::i a, float t)
{
	return color(a.r/t, a.g/t, a.b/t);
}

//MATERIAL
struct material {
	color dr; // odbicie rozproszone
	color sr; //odbicie zwierciadlane
	color st; //transmisja zwierciadlana
	float ior; //indeks refrakcji


	bool isDiffuse;
	bool isSpecular;
	bool isTransmittive;
	bool hasTextures;
	bool hasDrMap;
	bool hasNormalMap;

	
};

//ZDERZENIE
struct hit {
	float t;  // odległość trafienia od źródła promienia
	vec3 ip;  // punkt przecięcia
	vec3 N;   // wektor normalny do powierzchni trafionego obiektu
	vec3 Ex;  // wektor na osi X w przestrzeni rozpiętej na wektorach Ex, N, Ez
	vec3 Ez;  // wektor na osi Z w przestrzeni rozpiętej na wektorach Ex, N, Ez
	vec2 uv;  // współrzędne trafienia w przestrzeni tekstury
	material* mat; // wskaźnik na materiał

	hit() : t(undefined) {}
};
//OBIEKTY
struct object {
	string name;
	vec3 location;
	object() {};
	object(string nazwa, vec3 lokalizacja) :name(nazwa), location(lokalizacja) {}

};
struct sphere: object 
{
	float radius;


	sphere(string nazwa, vec3 lokalizacja, float promien) : radius(promien) { location = lokalizacja; name = nazwa; }
};
struct triangle: object
{
	

};
//kamera
struct camera:object
{
	nat resX, resY;
	
	camera(string nazwa, vec3 lokalizacja, nat x, nat y) :resX(x), resY(y) { location = lokalizacja; name = nazwa; }
	void przeszukaj()
	{
		location.x = location.x + resX;
		location.y = location.y + resY;
		for (int i = 0; i < resX; i++)
		{
			for (int j = 0; j < resY; i++)
			{
				vec3 zrodlo(location.x + i, location.y + j, location.z);
				vec3 kierunek(0, 1, 0);
				unitise(kierunek);
				ray promien(zrodlo, kierunek );
			}
		}
	}

};
//ZRODLO SWIATLA
struct lightSrc {
	vec3 location;
	color intensity;
	color ambient;
	lightSrc(vec3 location, color intensity):location(location), intensity(intensity), ambient(intensity/4.0f){}
};
//SCENA
struct scene
{
	vector <object> obiekty;
	vector <camera> kamery;
	vector <lightSrc> zrodlo_swiatla;
	color kolor_tla;

};

int main()
{
	scene scena;
	vec3 wektorek(1, 1, 1);
	sphere sferka("sfera", wektorek, 12);

	scena.obiekty.push_back(sferka);
}
