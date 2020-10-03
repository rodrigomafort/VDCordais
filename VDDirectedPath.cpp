#include "LocalLib.h"
#include <chrono>
#include <thread>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

set<Vertice> VDDirectedPath::Executar(Grafo* g, VetorRequisitos R, string &info, void* pars)
{
	VDDirectedPath sol;
	sol.Inicializar(g, R);
	set<Vertice> S = sol.Executar();
	info = sol.Info;
	return S;
}

void VDDirectedPath::Inicializar(Grafo* g, VetorRequisitos r)
{
	G = static_cast<GrafoCordal*>(g);
	R = r;
}

void PosOrdem(Grafo* G, NoClique* no, map<NoClique*, int> &depth, map<Vertice,int> &high, map<Vertice,int>& req, map<Vertice,bool> &dom, set<Vertice> &S)
{
	for(NoClique* f : no->filhos)
		PosOrdem(G,f,depth,high,req,dom,S);

	vector<Vertice> clique;
	for (Vertice v : no->vertices)
	{
		//Todo vértice que atende a condição do algoritmo é incluído no vetor clique
		if (req[v] > 0 && dom[v] == false && high[v] == depth[no])
		{
			clique.push_back(v);
		}
	}

	auto ordenarReq = [&](Vertice const &a, Vertice const &b) {
		return (req[a] > req[b]);
	};
	//Ordenar os vértices não domaindos da clique em ordem decrescente de requisitos
	sort(clique.begin(), clique.end(), ordenarReq);

	while (clique.size() > 0) //Enquanto houver vértice não dominado
	{
		Vertice v = clique.front();

		vector<pair<Vertice,pair<int,int>>> vetAdj;
		for (Vertice u : G->AdjacentesFechado(v))
		{
			if (binary_search(S.begin(), S.end(), u) == false)
				vetAdj.push_back(make_pair(u,make_pair(high[u],req[u])));
		}

		auto ordernarSelecao = [&](pair<Vertice,pair<int,int>> const &a, pair<Vertice,pair<int,int>> const &b) {
			if (a.second.first < b.second.first) return true;
			else if (a.second.first > b.second.first) return false;
			else return a.second.second > b.second.second;
		};

		sort(vetAdj.begin(), vetAdj.end(), ordernarSelecao);

		Vertice u = vetAdj.front().first;
		S.insert(u);

		req[u] = -1; //Marcando u como dominado
		for (Vertice w : G->AdjacentesFechado(u))
		{
			if (req[w] > 0)	req[w] = req[w] - 1;
			if (req[w] <= 0 && dom[w] == false)
			{
				dom[w] = true;
				vector<Vertice>::iterator it = find(clique.begin(), clique.end(), w);
				if (it != clique.end())  clique.erase(it);
			}
		}
	}
}

set<Vertice> VDDirectedPath::Executar()
{
	set<Vertice> sol;

	ArvoreCliques ac = G->ObterArvoreCliques();

	map<NoClique*,int> depth;
	map<Vertice,int> high;
	map<Vertice,bool> dom;
	map<Vertice,int> req;


	for (Vertice v : G->Vertices)
	{
		high[v] = -1;
		req[v] = R[v];
		if (req[v] > 0) dom[v] = false;
		else dom[v] = true;
	}

	depth[ac.raiz] = 0;
	for (Vertice v : ac.raiz->vertices)
	{
		high[v] = 0;
	}

	queue<NoClique*> fila;
	for(NoClique* filho : ac.raiz->filhos)
	{
		fila.push(filho);
	}

	while (fila.size() > 0)
	{
		NoClique* no = fila.front();
		fila.pop();

		depth[no] = depth[no->pai] + 1;
		for (Vertice v : no->vertices)
		{
			if (high[v] == -1) high[v] = depth[no];
		}

		for(NoClique* filho : no->filhos)
		{
			fila.push(filho);
		}
	}

	PosOrdem(G,ac.raiz,depth,high,req,dom,sol);

	return sol;
}
