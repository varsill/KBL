#include <cstdlib>
#include <limits>
#include <math.h>
#include <vector>
#include <string>
#include<windows.h>
#include<iostream>
#include <process.h>
#include <gl\GL.h>
using namespace std;
std::numeric_limits<float> real;
const float epsilon3 = 0.001f;
const float epsilon5 = 0.00001f;
const float infinity = real.infinity();
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
#define resolutionX 2000
#define resolutionY 1000
#define sizex 1000
#define sizey 500
#define maxDepth 3

COLORREF *  bitmap = new COLORREF[resolutionX*resolutionY];
inline float uRand() { return (fl(rand()) / fl(RAND_MAX)); }
int get_order(int x, int y)
{
	return (resolutionX*y + x);
}

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
		typedef const ray& i; // input ref
		vec3 origin;
		vec3 abackDir;
		vec3 aheadDir;

		ray(vec3 origin, vec3 aheadDir2) :
			origin(origin), aheadDir(aheadDir2), abackDir(invert(aheadDir2)) {}
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


//OBIEKTY
struct primitives {
	vec3 location;
};
struct object {
	string name;
	vec3 location;
	vector <vec3> vertexes;
	object() {};
	object(string nazwa, vec3 lokalizacja) :name(nazwa), location(lokalizacja) {}

};
struct sphere : primitives{
	float radius;
	sphere(vec3 lokalizacja, float promien) : radius(promien) { location = lokalizacja;}
};
struct triangle {
	typedef const triangle& i; // input ref

	vec3 v1, v2, v3;    // wierzchołki trójkąta
	vec2 uv1, uv2, uv3; // wierzchołki trójkąta w przestrzeni tekstury
	vec3 N;             // wektor normalny do płaszczyzny trójkąta
	float d;            // równanie płaszczyzny: dotProd(N,(x,y,z)) + d = 0
	material* mat;      // wskaźnik na materiał
	triangle(vec3 pos1, vec3 pos2, vec3 pos3):v1(pos1), v2(pos2), v3(pos3){
		N = uCrossProd(v2 - v1, v3 - v1);
		d = -dotProd(N, v1);
	
	}
		
						// konstruktory
};
struct hit {
	float t;  // odległość trafienia od źródła promienia
	vec3 ip;  // punkt przecięcia
	vec3 N;   // wektor normalny do powierzchni trafionego obiektu
	vec3 Ex;  // wektor na osi X w przestrzeni rozpiętej na wektorach Ex, N, Ez
	vec3 Ez;  // wektor na osi Z w przestrzeni rozpiętej na wektorach Ex, N, Ez
	vec2 uv;  // współrzędne trafienia w przestrzeni tekstury
	material* mat; // wskaźnik na materiał

	hit() : t(undefined) {}



	hit(triangle::i T, float t, ray::i r) : t(t) {
		if (t == undefined) return;
		ip = r.origin + r.aheadDir * t;  // punkt przecięcia
		mat = T.mat;                     // wskaźnik na materiał

	}
};
inline float area(vec3::i v1, vec3::i v2, vec3::i v3) {
	float a = length(v2 - v1);
	float b = length(v3 - v1);
	float c = length(v3 - v2);
	float p = (a + b + c) / 2.0f;
	return sqrt(p*(p - a)*(p - b)*(p - c));
}
inline float intersect(triangle::i T, ray::i r) {
	float t = (-T.d - dotProd(T.N, r.origin)) / dotProd(T.N, r.aheadDir);
	if (!(t > epsilon5 && t < infinity))
		return undefined;
	vec3 ip = r.origin + r.aheadDir * t;
	float abc = area(T.v1, T.v2, T.v3);
	float pbc = area(ip, T.v2, T.v3);
	float apc = area(T.v1, ip, T.v3);
	float abp = area(T.v1, T.v2, ip);
	if (pbc + apc + abp > abc + epsilon3)
		return undefined;
	return t;
}
//kamera
struct lightSrc {
	vec3 location;
	color intensity;
	color ambient;
	lightSrc(vec3 location, color intensity) :location(location), intensity(intensity), ambient(intensity / 4.0f) {}
};
struct scene
{
	vector <triangle> objects;
	vector <lightSrc> lightsources;
	color backbground_color;
	void add_object(vec3 pos1, vec3 pos2, vec3 pos3)
	{
		triangle to_be_added(pos1, pos2, pos3);
		objects.push_back(to_be_added);
	}
};
scene scena;
struct camera:object
{
	nat resX, resY;
	float t;
	float distance;
	camera(string nazwa, vec3 lokalizacja, nat x, nat y, float z) :resX(x), resY(y),  distance(z){ location = lokalizacja; name = nazwa; }
	void inline turn_on()
	{
		int i, j;
		vec3 current_location(0, 0, 0);
		vec3 dir = unitise(vec3(0, 0, 1));
		for (j = 0; j < resY; j++)
		{
			for (i = 0; i < resX; i++)
			{
				hit(i, j, 0);
			}
		}
	
		
	}
	void inline hit(int i, int j, int depth)
	{
		
		vec3 current_location = location + vec3(-(resolutionX / 2), resolutionY / 2, 0) + vec3(i, -j, distance)+vec3(uRand(), uRand(), uRand());
		vec3 dir = unitise(current_location - location);
		ray r(current_location, dir);
		float t = intersect(scena.objects[0], r);
		if (depth >= maxDepth)
		{
			return;
		}
		if ((t != infinity) && (t != 0))
		{
			*(bitmap + get_order(i, j)) = RGB(20, 13, 0);
			return;
		}

		*(bitmap + get_order(i, j)) = RGB(255, 255, 255);
		hit(i, j, depth + 1);
		return;
		
		

		
		

	}
};
//ZRODLO SWIATLA

//SCENA



BYTE* getPixArray(HBITMAP hBitmap)
{

	HDC hdc, hdcMem;

	hdc = GetDC(NULL);
	hdcMem = CreateCompatibleDC(hdc);

	BITMAPINFO MyBMInfo = { 0 };

	MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);

	if (0 == GetDIBits(hdcMem, hBitmap, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS))
	{
		return 0;
	}

	BYTE* lpPixels = new BYTE[MyBMInfo.bmiHeader.biSizeImage];

	MyBMInfo.bmiHeader.biBitCount = 32;
	MyBMInfo.bmiHeader.biCompression = BI_RGB;
	MyBMInfo.bmiHeader.biHeight = (MyBMInfo.bmiHeader.biHeight < 0) ? (-MyBMInfo.bmiHeader.biHeight) : (MyBMInfo.bmiHeader.biHeight);


	if (0 == GetDIBits(hdcMem, hBitmap, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)lpPixels, &MyBMInfo, DIB_RGB_COLORS))
	{
		return 0;
	}
	return lpPixels;
}

//NOWY_WATEK -  do aktualizowania ekranu! - ino dla listenerów zmian

void aktualizuj(HDC ddc)
{
	
	//
	//
	BITMAP * bmInfo;
	HDC * hdc = new HDC();
	*hdc = CreateCompatibleDC(NULL);
	//HBITMAP hbm = (HBITMAP)LoadImage(NULL, "D:\\bitmap.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	/*for (int i = 0; i < resolutionX*resolutionY; i++)
	{
		b[i] = bitmap[i];
	}
	*/
	HBITMAP hbm = CreateBitmap(resolutionX, resolutionY, 1,32, (LPVOID)bitmap);
	SelectObject(*hdc, hbm);
    GetObject(hbm, sizeof(bmInfo), &bmInfo);
	SetBitmapBits(hbm, resolutionX * resolutionY, (LPVOID)bitmap);
	BitBlt(ddc, 0, 0, resolutionX, resolutionY, *hdc, 0, 0, SRCCOPY);

}
void __cdecl update_screen(COLORREF * tablica, HDC hdc)
{
	//aktualizuj(tablica, hdc);

}

/*vec2 viewporting(vec3 pos, camera cam) {
	vec3 nw = pos+(cam.location - vec3(0, 0, 0));
	vec2 ret((nw.x*cam.distance)/(nw.z+cam.distance), (nw.y*cam.distance) / (nw.z + cam.distance));
	return ret;
}
void viewport(triangle tr, camera cam)
{
	float axis, h;
	vec2 pos1 = viewporting(tr.wierzcholki[0], cam);
	vec2 pos2 = viewporting(tr.wierzcholki[1], cam);
	vec2 pos3 = viewporting(tr.wierzcholki[2], cam);
	if (pos1.v >= pos2.v&&pos1.v >= pos2.v)
	{

		axis = pos1.v;
		if (pos2.v >= pos3.v)
		{
			h = axis - pos3.v;
		}
		else
		{
			h = axis - pos2.v;
		}
	}
	else if (pos2.v >= pos1.v&&pos2.v >= pos3.v)
	{
		axis = pos2.v;
		if (pos1.v >= pos3.v)
		{
			h = axis - pos3.v;
		}
		else
		{
			h = axis - pos1.v;
		}
	}
	else
	{
		axis = pos3.v;
		if (pos1.v >= pos2.v)
		{
			h = axis - pos2.v;
		}
		else
		{
			h = axis - pos1.v;
		}
	}

}
void find_viewport(scene * scn, COLORREF * tab)
{
	for (int i = 0; i < scn->objects.size(); i++)
	{
		for (int j = 0; j < scn->objects[i].vertexes.size(); j++)
		{
			vec2 pos = viewporting(scn->objects[i].vertexes[i], scn->cameras[0], tab );
		}
	}
	
}
*/
/*int main()
{
	
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);
	for (int i = 0; i < 10000; i++) {
		find_viewport(vec3(i, i, i), 1, hdc);
	}
	

	system("PAUSE");



}
*/



LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;
HWND g_hPrzycisk;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// WYPEŁNIANIE STRUKTURY
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = NazwaKlasy;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// REJESTROWANIE KLASY OKNA
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
			MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	// TWORZENIE OKNA
	HWND hwnd;

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, NazwaKlasy, "KaBeL Corporation 2016", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, resolutionX, resolutionY, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Okno odmówiło przyjścia na świat!", "Ale kicha...", MB_ICONEXCLAMATION);
		return 1;
	}
	g_hPrzycisk = CreateWindowEx(0, "BUTTON", "RENDER", WS_CHILD | WS_VISIBLE,
		100, 100, 150, 30, hwnd, NULL, hInstance, NULL);

	

	ShowWindow(hwnd, nCmdShow); // Pokaż okienko...
	UpdateWindow(hwnd);

	// Pętla komunikatów
	while (GetMessage(&Komunikat, NULL, 0, 0))
	{
		TranslateMessage(&Komunikat);
		DispatchMessage(&Komunikat);
	}
	return Komunikat.wParam;
}

// OBSŁUGA ZDARZEŃ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
	{
		scena.add_object(vec3(120, 200, 150), vec3(780, 200, 120), vec3(120, 500, 150));
		HDC hdc = GetDC(hwnd);
		camera cam("kamerka", vec3(-200,0 , 0 ), 2000, 1000, 100);
		cam.turn_on();
		
		aktualizuj(hdc);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}
