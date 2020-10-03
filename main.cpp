#include "LocalLib.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <chrono>
#include <thread>

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

void ExecutarValidadorExaustivo()
{
	GrafoSplitIndiferenca* G = new GrafoSplitIndiferenca(5);

	ValidadorAlgoritmosExaustivo* val = new ValidadorAlgoritmosExaustivo(
			VDGrafoSplitIndiferenca::Executar,
			VetorRequisitos::VDAvaliarDominacao,
			VDCPLEX::BuscarSolucao,
			G,
			"testeExaustivo.txt",
			3);

	val->Executar();
}

void ExecutarValidadorRandomico()
{
	GrafoSplitIndiferenca* G = new GrafoSplitIndiferenca(5);

	ValidadorAlgoritmosRandomico* val = new ValidadorAlgoritmosRandomico(
			VDGrafoSplitIndiferenca::Executar,
			VetorRequisitos::VDAvaliarDominacao,
			VDCPLEX::BuscarSolucao,
			G,
			"testeRandomico.txt",
			500,
			3);

	val->Executar();
}

void ExecutarExemplo()
{
	int** mat = new int*[7];
	int ct = 0;
	mat[ct++] = new int[7] { 0, 1, 1, 1, 1, 0, 0};
	mat[ct++] = new int[7] { 1, 0, 1, 1, 1, 1, 0};
	mat[ct++] = new int[7] { 1, 1, 0, 1, 1, 1, 1};
	mat[ct++] = new int[7] { 1, 1, 1, 0, 1, 1, 1};
	mat[ct++] = new int[7] { 1, 1, 1, 1, 0, 1, 1};
	mat[ct++] = new int[7] { 0, 1, 1, 1, 1, 0, 1};
	mat[ct++] = new int[7] { 0, 0, 1, 1, 1, 1, 0};
	
	GrafoSplitIndiferenca* G = new GrafoSplitIndiferenca(7,mat);
	cout << G->ObterDetalhes();
	
	
	VetorRequisitos R(new int[7]{3, 3, 2, 2, 2, 3, 3},G);	
	string info;
	set<Vertice> resposta = VDGrafoSplitIndiferenca::Executar(G,R,info,NULL);	
	cout << Grafo::ObterString(resposta) << endl;
}

int main(int argc, char** argv)
{
	ExecutarValidadorExaustivo();
	ExecutarValidadorRandomico();
	ExecutarExemplo();
	return 0;
}
