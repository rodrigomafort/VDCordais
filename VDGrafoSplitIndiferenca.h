#include "LocalLib.h"
#pragma once

//Implementação do Algoritmo publicado na Information Processing Letters - Volume 155 (Março 2020)
//https://www.sciencedirect.com/science/article/abs/pii/S0020019019301826

class VDGrafoSplitIndiferenca
{
	GrafoSplitIndiferenca* G;
	VetorRequisitos R2;
	string Info;

	set<Vertice> ResolverCaso1(set<Vertice> C, VetorRequisitos R);
	set<Vertice> ResolverCaso1Mod(set<Vertice> C, Vertice v, VetorRequisitos R);
	set<Vertice> ResolverCaso2(set<Vertice> C1, set<Vertice> C2, VetorRequisitos R);
	set<Vertice> ResolverCaso2Mod(set<Vertice> C2, Vertice v, VetorRequisitos R);
	set<Vertice> ResolverCaso3(set<Vertice> C1, set<Vertice> C2, set<Vertice> C3, VetorRequisitos R);
	set<Vertice> ResolverCaso4(set<Vertice> C1, set<Vertice> C2, set<Vertice> C3, VetorRequisitos R);
	set<Vertice> Reduzir(set<Vertice> S, set<Vertice> C2, Vertice v, Vertice w, VetorRequisitos R);
	set<Vertice> Trocar(set<Vertice> S, set<Vertice>C1, set<Vertice> C2, set<Vertice> C3, Vertice v, Vertice w, VetorRequisitos R);
	void Inicializar(Grafo* G, VetorRequisitos R);
	set<Vertice> Executar();

public:
	static set<Vertice> Executar(Grafo* g, VetorRequisitos R, string &info, void* pars);
};

