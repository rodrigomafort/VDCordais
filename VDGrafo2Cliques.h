#pragma once
#include "LocalLib.h"

class VDGrafo2Cliques
{
	Grafo2Cliques* G;
	VetorRequisitos F;
	string Info;
	
	vector<Vertice> ResolverGrafo(vector<Vertice> C1, vector<Vertice> C2, vector<Vertice> inter, VetorRequisitos f);
	vector<Vertice> OtimizarSolucao(vector<Vertice> C1_total, vector<Vertice> C2_total, vector<Vertice> intersecao, VetorRequisitos f, vector<Vertice> S);

	void Inicializar(Grafo* G, VetorRequisitos F);
	vector<Vertice> Executar();

public:
	static vector<Vertice> Executar(Grafo* g, VetorRequisitos R, string &info, void* pars);
	
};
