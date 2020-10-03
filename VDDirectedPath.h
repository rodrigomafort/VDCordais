#pragma once
#include "LocalLib.h"

class VDDirectedPath
{
	GrafoCordal* G;
	VetorRequisitos R;
	string Info;

	void Inicializar(Grafo* G, VetorRequisitos R);
	set<Vertice> Executar();

	public:
		static set<Vertice> Executar(Grafo* g, VetorRequisitos R, string &info, void* pars);
};
