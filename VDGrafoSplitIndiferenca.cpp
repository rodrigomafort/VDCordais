#include "LocalLib.h"

//Implementação do Algoritmo publicado na Information Processing Letters - Volume 155 (Março 2020)
//https://www.sciencedirect.com/science/article/abs/pii/S0020019019301826

set<Vertice> VDGrafoSplitIndiferenca::Executar(Grafo* g, VetorRequisitos R, string &info, void* pars)
{
	VDGrafoSplitIndiferenca sol;
	sol.Inicializar(g, R);
	set<Vertice> S = sol.Executar();
	info = sol.Info;
	return S;
}

void VDGrafoSplitIndiferenca::Inicializar(Grafo* g, VetorRequisitos r)
{
	G = static_cast<GrafoSplitIndiferenca*>(g);
	R2 = r;
}

set<Vertice> VDGrafoSplitIndiferenca::Executar()
{
	DetalhesSplitIndiferenca detalhado = G->Detalhar();	
	
	ArvoreCliques AC = G->ObterArvoreCliques();

	if (detalhado.Caso <= 2 || AC.cliques.size() == 2)
	{
		DetalhesSplitIndiferenca detalhado2 = G->Detalhar();
	}
	
	set<Vertice> S;
	Info = "";
	switch ( detalhado.Caso)
	{
		case 1: S = ResolverCaso1(detalhado.C1, R2); break;
		case 2: S = ResolverCaso2(detalhado.C1, detalhado.C2, R2); break;
		case 3: S = ResolverCaso3(detalhado.C1, detalhado.C2, detalhado.C3, R2); break;
		case 4: S = ResolverCaso4(detalhado.C1, detalhado.C2, detalhado.C3, R2); break;
	}

	return S;
}

set<Vertice> VDGrafoSplitIndiferenca::ResolverCaso1(set<Vertice> C, VetorRequisitos R)
{
	auto FDecrescente = [&](Vertice a, Vertice b)-> bool {
		return R[a] > R[b];
	};

	vector<Vertice> cand(C.begin(), C.end());
	sort(cand.begin(), cand.end(), FDecrescente);

	set<Vertice> S;
	while (cand.size() > 0)
	{
		Vertice v = cand.front();
		cand.erase(cand.begin());

		if (R[v] > S.size())
		{
			S.insert(v);
		}
		else
		{
			break;
		}
	}
	return S;
}

set<Vertice> VDGrafoSplitIndiferenca::ResolverCaso1Mod(set<Vertice> C, Vertice esp, VetorRequisitos R)
{
	auto FDecrescente = [&](Vertice a, Vertice b)-> bool {
		return R[a] > R[b];
	};

	vector<Vertice> cand(C.begin(), C.end());
	sort(cand.begin(), cand.end(), FDecrescente);

	set<Vertice> S;
	while (cand.size() > 0)
	{
		Vertice v = cand.front();
		cand.erase(cand.begin());
		if (v == esp) continue;
		
		if (R[v] > S.size() || R[esp] > S.size())
		{
			S.insert(v);
		}
		else
		{
			break;
		}
	}
	return S;
}

set<Vertice> VDGrafoSplitIndiferenca::ResolverCaso2(set<Vertice> C1, set<Vertice> C2, VetorRequisitos R)
{
	set<Vertice> S1;
	set<Vertice> S2;

	set<Vertice> setDiff;
	set_difference(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(setDiff, setDiff.end()));
	Vertice v = *setDiff.begin();

	//Caso a: v in S
	S1.insert(v);
	VetorRequisitos R1(R);
	for (Vertice u : G->Adjacentes(v))
	{		
		R1[u] = R1[u] - 1;
		if (R1[u] < 0) R1[u] = 0;			
	}
	set<Vertice> sol1 = ResolverCaso1(C2, R1);
	S1.insert(sol1.begin(), sol1.end());

	//Caso b : v not in S
	set<Vertice> aTemp = G->Adjacentes(v);
	vector<Vertice> adjv(aTemp.begin(), aTemp.end());
	auto FDecrescente = [&](Vertice a, Vertice b)-> bool { return R[a] > R[b]; };
	sort(adjv.begin(), adjv.end(), FDecrescente);
	S2.insert(adjv.begin(), adjv.begin() + R[v]);
	VetorRequisitos R2(R);
	for (Vertice u : C2)
	{
		R2[u] = R2[u] - S2.size();
		if (R2[u] < 0) R2[u] = 0;
	}
	
	set<Vertice> C2Red;
	set_difference(C2.begin(), C2.end(), S2.begin(), S2.end(), inserter(C2Red, C2Red.end()));	
	set<Vertice> sol2 = ResolverCaso1(C2Red, R2);
	S2.insert(sol2.begin(), sol2.end());

	if (S1.size() <= S2.size())
	{
		return S1;
	}
	else
	{
		return S2;
	}
}

set<Vertice> VDGrafoSplitIndiferenca::ResolverCaso2Mod(set<Vertice> C2, Vertice v, VetorRequisitos R)
{
	set<Vertice> S;
	
	set<Vertice> aTemp = G->Adjacentes(v);
	vector<Vertice> adjv(aTemp.begin(), aTemp.end());
	auto FDecrescente = [&](Vertice a, Vertice b)-> bool { return R[a] > R[b]; };
	sort(adjv.begin(), adjv.end(), FDecrescente);
	S.insert(adjv.begin(), adjv.begin() + R[v]);
	VetorRequisitos R2(R);
	for (Vertice u : C2)
	{
		R2[u] = R2[u] - S.size();
		if (R2[u] < 0) R2[u] = 0;
	}

	set<Vertice> C2Red;
	set_difference(C2.begin(), C2.end(), S.begin(), S.end(), inserter(C2Red, C2Red.end()));
	set<Vertice> sol2 = ResolverCaso1(C2Red, R2);
	S.insert(sol2.begin(), sol2.end());

	return S;
}

set<Vertice> VDGrafoSplitIndiferenca::ResolverCaso3(set<Vertice> C1, set<Vertice> C2, set<Vertice> C3, VetorRequisitos R)
{
	set<Vertice> S1;
	set<Vertice> S2;

	set<Vertice> setDiff;
	set_difference(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(setDiff, setDiff.end()));
	Vertice v = *setDiff.begin();

	//Caso a) v contido em S
	S1.insert(v);
	VetorRequisitos R1 = R;
	for (Vertice u : G->Adjacentes(v))
	{
		R1[u] = R1[u] - 1;
		if (R1[u] < 0) R1[u] = 0;
	}
	set<Vertice> SolRest1 = ResolverCaso2(C3, C2, R1);
	S1.insert(SolRest1.begin(), SolRest1.end());

	//Caso b) v n�o contido em S
	set<Vertice> aTemp = G->Adjacentes(v);
	vector<Vertice> adjv(aTemp.begin(), aTemp.end());
	auto FDecrescente = [&](Vertice a, Vertice b)-> bool { return R[a] > R[b]; };
	sort(adjv.begin(), adjv.end(), FDecrescente);
	S2.insert(adjv.begin(), adjv.begin() + R[v]);
	VetorRequisitos R2 = R;
	for (Vertice u : C2)
	{
		R2[u] = R2[u] - S2.size();
		if (R2[u] < 0) R2[u] = 0;
	}

	set<Vertice> C2Red;
	set_difference(C2.begin(), C2.end(), S2.begin(), S2.end(), inserter(C2Red, C2Red.end()));
	set<Vertice> SolRest2 = ResolverCaso2(C3, C2Red, R2);
	S2.insert(SolRest2.begin(), SolRest2.end());

	if (S1.size() <= S2.size())
	{
		return S1;
	}
	else
	{
		return S2;
	}
}

set<Vertice> VDGrafoSplitIndiferenca::ResolverCaso4(set<Vertice> C1, set<Vertice> C2, set<Vertice> C3, VetorRequisitos R)
{
	set<Vertice> S1;
	set<Vertice> S2;
	set<Vertice> S3;
	set<Vertice> S4;
	set<Vertice> SMin;

	set<Vertice> setDiff1;
	set_difference(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(setDiff1, setDiff1.end()));
	Vertice v = *setDiff1.begin();

	set<Vertice> setDiff3;
	set_difference(C3.begin(), C3.end(), C2.begin(), C2.end(), inserter(setDiff3, setDiff3.end()));
	Vertice w = *setDiff3.begin();

	//Casos a) v e w contidos em S
	S1.insert(v);
	S1.insert(w);
	VetorRequisitos R1 = R;
	for (Vertice u : G->Adjacentes(v))
	{
		R1[u] = R1[u] - 1;
		if (R1[u] < 0) R1[u] = 0;
	}
	for (Vertice u : G->Adjacentes(w))
	{
		R1[u] = R1[u] - 1;
		if (R1[u] < 0) R1[u] = 0;
	}
	set<Vertice> SolRest1 = ResolverCaso1(C2, R1);
	S1.insert(SolRest1.begin(), SolRest1.end());
	SMin = S1;
	
	//Caso b) v contido e w n�o contido em S
	S2.insert(v);
	VetorRequisitos R2 = R;
	for (Vertice u : G->Adjacentes(v))
	{
		R2[u] = R2[u] - 1;
		if (R2[u] < 0) R2[u] = 0;
	}
	set<Vertice> SolRes2 = ResolverCaso2Mod(C2, w, R2);
	S2.insert(SolRes2.begin(), SolRes2.end());
	if (S2.size() < SMin.size()) SMin = S2;

	//Caso c) w contido e v n�o contido em S
	S3.insert(w);
	VetorRequisitos R3 = R;
	for (Vertice u : G->Adjacentes(w))
	{
		R3[u] = R3[u] - 1;
		if (R3[u] < 0) R3[u] = 0;
	}
	set<Vertice> SolRes3 = ResolverCaso2Mod(C2, v, R3);
	S3.insert(SolRes3.begin(), SolRes3.end());
	if (S3.size() < SMin.size()) SMin = S3;
	
	//Caso d) v n�o contido em S
	set<Vertice> solC1 = ResolverCaso1Mod(C1, v, R);
	set<Vertice> solC3 = ResolverCaso1Mod(C3, w, R);
	set<Vertice> solTotal;
	set_union(solC1.begin(), solC1.end(), solC3.begin(), solC3.end(), inserter(solTotal, solTotal.end()));

	solTotal = Reduzir(solTotal, C2, v, w, R);
	solTotal = Trocar(solTotal, C1, C2, C3, v, w, R);
	//solTotal = Reduzir(solTotal, C2, v, w, R);
	S4 = solTotal;

	if (S4.size() < SMin.size()) SMin = S4;

	return SMin;
}

set<Vertice> VDGrafoSplitIndiferenca::Reduzir(set<Vertice> S, set<Vertice> C2, Vertice v, Vertice w, VetorRequisitos R)
{
	auto RDecrescente = [&](Vertice a, Vertice b)-> bool { return R[a] > R[b]; };
	auto RCrescente = [&](Vertice a, Vertice b)-> bool { return R[a] < R[b]; };
	
	vector<Vertice> cand(S.begin(), S.end());
	sort(cand.begin(), cand.end(), RCrescente);

	vector<Vertice> dominadosS;
	set_difference(C2.begin(), C2.end(), S.begin(), S.end(), inserter(dominadosS, dominadosS.end()));
	sort(dominadosS.begin(), dominadosS.end(), RDecrescente);
	Vertice maxR;
	bool cmax = false;
	if (dominadosS.size() > 0)
	{
		cmax = true;
		maxR = dominadosS.front();
	}

	map<Vertice, bool> adjvS;
	map<Vertice, bool> adjwS;

	int ctvS = 0;
	int ctwS = 0;

	for (Vertice u : C2)
	{
		if (binary_search(S.begin(), S.end(), u))
		{
			if (G->VerificarAdjacencia(v, u))
			{
				adjvS.insert(pair<Vertice, bool>(u, true));
				ctvS++;
			}
			else
			{
				adjvS.insert(pair<Vertice, bool>(u, false));
			}

			if (G->VerificarAdjacencia(w, u))
			{
				adjwS.insert(pair<Vertice, bool>(u, true));
				ctwS++;
			}
			else
			{
				adjwS.insert(pair<Vertice, bool>(u, false));
			}
		}
		else
		{
			adjvS.insert(pair<Vertice, bool>(u, false));
			adjwS.insert(pair<Vertice, bool>(u, false));
		}		
	}
	for (Vertice u : cand)
	{
		if (R[u] < S.size() && (cmax == false || R[maxR] < S.size()) && (adjvS[u] == false || R[v] < ctvS) && (adjwS[u] == false || R[w] < ctwS))
		{
			//u pode ser removido
			S.erase(u);
			if (adjvS[u] == true) {
				ctvS--;
				adjvS[u] = false;
			}
			if (adjwS[u] == true)
			{
				ctwS--;
				adjwS[u] = false;
			}

			if (cmax == false)
			{
				cmax = true;
				maxR = u;
			}
			else
			{
				if (R[u] > R[maxR]) maxR = u;
			}
			
			Info= Info + "R("+to_string(u.Id()) + ") ";
		}
		else
		{
			Info = Info + "R(";
			if (R[u] >= S.size())
				Info = Info + "1,";
			else
				Info = Info + "0,";
			
			if ((cmax == true && R[maxR] >= S.size()))
				Info = Info + "1,";
			else
				Info = Info + "0,";
			
			if (adjvS[u] == true && R[v] >= ctvS)
				Info = Info + "1,";
			else
				Info = Info + "0,";
			
			if (adjwS[u] == true && R[w] >= ctwS)
				Info = Info + "1,";
			else
				Info = Info + "0,";			
			
			if ((cmax == true && R[maxR] >= S.size()))
				Info = Info + to_string(maxR.Id());
			else
				Info = Info + "-";
			
			Info = Info + ") ";
		}		
	}

	return S;
}

set<Vertice> VDGrafoSplitIndiferenca::Trocar(set<Vertice> S, set<Vertice>C1, set<Vertice> C2, set<Vertice> C3, Vertice v, Vertice w, VetorRequisitos R)
{
	set<Vertice> adjv;
	set<Vertice> adjw;
	set_difference(C1.begin(), C1.end(), C3.begin(), C3.end(), inserter(adjv, adjv.end()));	
	set_difference(C3.begin(), C3.end(), C1.begin(), C1.end(), inserter(adjw, adjw.end()));
	adjv.erase(v);
	adjw.erase(w);

	vector<Vertice> adjvS;
	vector<Vertice> adjwS;
	set_intersection(adjv.begin(), adjv.end(), S.begin(), S.end(), inserter(adjvS, adjvS.end()));
	set_intersection(adjw.begin(), adjw.end(), S.begin(), S.end(), inserter(adjwS, adjwS.end()));

	set<Vertice> inter;
	set_intersection(C1.begin(), C1.end(), C3.begin(), C3.end(), inserter(inter, inter.end()));

	vector<Vertice> interCand;
	set_difference(inter.begin(), inter.end(), S.begin(), S.end(), inserter(interCand, interCand.end()));

	set<Vertice> aux;
	vector<Vertice> domC2;
	set_difference(C2.begin(), C2.end(), S.begin(), S.end(), inserter(aux, aux.end()));	
	set_difference(aux.begin(), aux.end(), interCand.begin(), interCand.end(), inserter(domC2, domC2.end()));
	
	auto RDecrescente = [&](Vertice a, Vertice b)-> bool { return R[a] > R[b]; };
	auto RCrescente = [&](Vertice a, Vertice b)-> bool { return R[a] < R[b]; };
	
	sort(adjvS.begin(), adjvS.end(), RCrescente);
	sort(adjwS.begin(), adjwS.end(), RCrescente);
	sort(interCand.begin(), interCand.end(), RDecrescente);
	sort(domC2.begin(), domC2.end(), RDecrescente);
	bool cmax = false;
	Vertice maxR;
	if (domC2.size() > 0)
	{
		maxR = domC2.front();
		cmax = true;
	}
	
	//Ideia: Trocar v�rtices de adjvS e adjwS por interCand
	while (adjvS.size() > 0 && adjwS.size() > 0 && interCand.size() > 0)
	{
		Vertice r1 = adjvS.front();
		adjvS.erase(adjvS.begin());
		Vertice r3 = adjwS.front();
		adjwS.erase(adjwS.begin());
		Vertice a = interCand.front();
		interCand.erase(interCand.begin());

		//Remover r1 e r3, adicionar a
		if (R[r1] <= S.size() - 1 && R[r3] <= S.size() - 1 && (cmax == false || R[maxR] <= S.size() - 1) && (interCand.empty() || R[interCand.front()] <= S.size() - 1))
		{
			//Troca pode ser realizada
			S.erase(r1);
			S.erase(r3);
			S.insert(a);
			Info = Info + "T(" + to_string(r1.Id()) + "," + to_string(r3.Id()) + "," + to_string(a.Id()) + ") ";
			if (cmax == false)
			{
				cmax = true;
				if (R[r1] > R[r3]) maxR = r1; else maxR = r3;
			}
			else
			{
				if (R[r1] > R[maxR]) maxR = r1;
				if (R[r3] > R[maxR]) maxR = r3;
			}
		}
		else
		{
			//N�o existem mais trocas poss�veis
			break;
		}
	}

	return S;
}
