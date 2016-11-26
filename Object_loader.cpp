#include <fstream>
#include <string>
#include <vector>
#include "Source.h"
using namespace std;
fstream plik;

void wczytaj(string sciezka)
{
	string typ, nazwa;
	string bufor;
	int i, ile_krawedzi, ile_wierzcholkow;
	float x, y, z;
	plik.open(sciezka);
	plik>>nazwa;
	plik >> typ;
	plik >> ile_wierzcholkow;
	if (typ == "xgon")
	{
		plik >> bufor;
		i = stoi(bufor);
		ile_krawedzi = (i*(i - 1)) / 2;
	}
	vector <triangle> triangles;
	vector <vec3> vertics;
	vector <int> edges;
	for (int i = 0; i < ile_wierzcholkow; i++)
	{
		plik >> x;
		plik >> y;
		plik >> z;
		vec3 wierzcholek(x, y, z);
		vertics.push_back(wierzcholek);

	}
	plik >> bufor;
	
	for (i = 0; i < ile_wierzcholkow; i++)
	{
		plik >> bufor;
		while (bufor != ";")
		{
			edges.push_back(stoi(bufor));
			plik >> bufor;
		}
		ile_krawedzi == ((edges.size())*(edges.size() - 1)) / 2;
		for (int j = 0; j < ile_krawedzi-1; j++)
		{
			for (int k = j + 1; k < ile_krawedzi; k++)
			{
				vec3 cords[3];
				cords[0] = vertics[i];
				cords[1] = vertics[edges[j]];
				cords[2] = vertics[edges[k]];
				triangle Triangle(cords);
				triangles.push_back(Triangle);
			}
		}
	}
}


