#include "LocalLib.h"

vector<Vertice> VDGrafo2Cliques::Executar(Grafo* g, VetorRequisitos R, string &info, void* pars)
{
	VDGrafo2Cliques sol;
	sol.Inicializar(g, R);
	vector<Vertice> S = sol.Executar();
	info = sol.Info;
	return S;
}

void VDGrafo2Cliques::Inicializar(Grafo* g, VetorRequisitos f)
{
	G = static_cast<Grafo2Cliques*>(g);
	F = f;
}

vector<Vertice> VDGrafo2Cliques::ResolverGrafo(vector<Vertice> C1, vector<Vertice> C2, vector<Vertice> inter, VetorRequisitos f)
{
	vector<Vertice> S;

	set_difference(C1.begin(), C1.end(), inter.begin(), inter.end(), inserter(S, S.end()));
	set_difference(C2.begin(), C2.end(), inter.begin(), inter.end(), inserter(S, S.end()));

	vector<Vertice> interCand(inter.begin(), inter.end());

	auto FDecrescente = [&](Vertice a, Vertice b)-> bool {
		return f[a] > f[b];
	};

	sort(interCand.begin(), interCand.end(), FDecrescente);

	int ct = 0;
	while (ct < interCand.size())
	{
		if (f[interCand[ct]] > S.size())
		{
			S.push_back(interCand[ct]);
		}
		else
		{
			break;
		}

		ct++;
	}
	
	return S;
}



vector<Vertice> VDGrafo2Cliques::OtimizarSolucao(vector<Vertice> C1_total, vector<Vertice> C2_total, vector<Vertice> intersecao, VetorRequisitos F, vector<Vertice> S)
{
	auto FDecrescente = [&](Vertice a, Vertice b)-> bool {
		return F[a] > F[b];
	};

	auto FCrescente = [&](Vertice a, Vertice b)-> bool {
		return F[a] < F[b];
	};

	vector<Vertice> C1;
	set_difference(C1_total.begin(), C1_total.end(), intersecao.begin(), intersecao.end(), inserter(C1, C1.end()));
	
	vector<Vertice> C2;
	set_difference(C2_total.begin(), C2_total.end(), intersecao.begin(), intersecao.end(), inserter(C2, C2.end()));

	vector<Vertice> S1;
	set_intersection(S.begin(), S.end(), C1.begin(), C1.end(), inserter(S1, S1.end()));
	sort(S1.begin(), S1.end(), FCrescente);

	vector<Vertice> S2;
	set_intersection(S.begin(), S.end(), C2.begin(), C2.end(), inserter(S2, S2.end()));
	sort(S2.begin(), S2.end(), FCrescente);

	vector<Vertice> SI;
	set_intersection(S.begin(), S.end(), intersecao.begin(), intersecao.end(), inserter(SI, SI.end()));
	

	vector<Vertice> inter;
	set_difference(intersecao.begin(), intersecao.end(), S.begin(), S.end(), inserter(inter, inter.end()));
	sort(inter.begin(), inter.end(), FDecrescente);

	vector<Vertice> SMin = S;
	
	int maxRmC1 = 0;
	int maxRmC2 = 0;

	Vertice v1;
	bool impedimentoV1;

	Vertice v2;
	bool impedimentoV2;

	Info = "";
	do
	{
		//Avaliar a remoção de v1 e v2
		if (S1.size() > 0)
		{
			v1 = S1.front();
			impedimentoV1 = (F[v1] >= S1.size() + SI.size()) || (maxRmC1 >= S1.size() + SI.size());
			if (inter.size() > 0)	impedimentoV1 = impedimentoV1 || F[inter.front()] >= S.size();
		}
		else
		{
			impedimentoV1 = true;
		}
		
		if (S2.size() > 0)
		{
			v2 = S2.front();
			impedimentoV2 = (F[v2] >= S2.size() + SI.size()) || (maxRmC2 >= S2.size() + SI.size());			
			if (inter.size() > 0)	impedimentoV2 = impedimentoV2 || F[inter.front()] >= S.size();
		}
		else
		{
			impedimentoV2 = true;
		}
		
		

		if (impedimentoV1 == false && impedimentoV2 == false && inter.size() > 0)
		{
			//Necessário avaliar se os dois podem ser removidos
			if (F[inter.front()] > S.size() - 2)
			{
				//Não é possível remover ambos os vértices, pois o vértice da interseção não seria convertido
				//Solução, proibir a remoção de um dos vértices. 
				if (F[v1] > F[v2])
				{
					impedimentoV1 = true;
				}
				else
				{
					impedimentoV2 = true;
				}
			}
		}

		if (impedimentoV1 == false)
		{
			S1.erase(remove(S1.begin(), S1.end(), v1), S1.end());
			S.erase(remove(S.begin(), S.end(), v1), S.end());
			maxRmC1 = F[v1];
			Info = Info + "1";
		}
		else
		{
			Info = Info + "0";
		}

		if (impedimentoV2 == false)
		{
			S2.erase(remove(S2.begin(), S2.end(), v2), S2.end());
			S.erase(remove(S.begin(), S.end(), v2), S.end());
			maxRmC2 = F[v2];
			Info = Info + "1";
		}
		else
		{
			Info = Info + "0";
		}

		if (impedimentoV1 == true && impedimentoV2 == true)
		{
			if (inter.size() > 0)
			{
				Vertice i = inter.front();
				Info = Info + "1";
				
				
				S.push_back(i);				
				SI.push_back(i);
				inter.erase(remove(inter.begin(), inter.end(), i), inter.end());

				sort(S.begin(), S.end());

				//novas remoções podem ter sido desempedidas
				impedimentoV1 = false;
				impedimentoV2 = false;
			}
			else
			{
				Info = Info + "0";
			}
		}
		else
		{
			Info = Info + "0";
		}

		Info = Info + " ";

		if (S.size() < SMin.size())
		{
			SMin = S;
		}
	}
	while (impedimentoV1 == false || impedimentoV2 == false);

	return SMin;
}

vector<Vertice> VDGrafo2Cliques::Executar()
{
	auto FDecrescente = [&](Vertice a, Vertice b)-> bool {
		return F[a] > F[b];
	};

	auto FCrescente = [&](Vertice a, Vertice b)-> bool {
		return F[a] < F[b];
	};

	//vector<Vertice> S;

	ArvoreCliques ac = G->ObterArvoreCliques();
	vector<Vertice> C1 = vector<Vertice>(ac.cliques[0]->vertices.begin(), ac.cliques[0]->vertices.end());
	vector<Vertice> C2 = vector<Vertice>(ac.cliques[1]->vertices.begin(), ac.cliques[1]->vertices.end());

	sort(C1.begin(), C1.end());
	sort(C2.begin(), C2.end());

	vector<Vertice> intersecao;
	set_intersection(C1.begin(), C1.end(), C2.begin(), C2.end(), inserter(intersecao, intersecao.end()));

	vector<Vertice> S = ResolverGrafo(C1, C2, intersecao, F);
	sort(S.begin(), S.end());
	return OtimizarSolucao(C1, C2, intersecao, F, S);

}
