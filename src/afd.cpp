#include "afd.h"
#include <fstream>

using namespace std;

AFD::~AFD() {
	list<Estado*>::iterator iEst;
	list<Transicao*>::iterator iTran;

	for (iEst = this->estados.begin(); iEst != this->estados.end(); ++iEst)
		delete *iEst;

	for (iTran = this->transicoes.begin(); iTran != this->transicoes.end(); ++iTran)
		delete *iTran;
}

AFD::AFD(std::string nome, std::list<Estado*> estados, std::list<Transicao*> transicoes, Estado *estadoInicial, 
	std::list<Estado*> estadosFinais) : nome(nome), estados(estados), transicoes(transicoes) {
	list<Estado*>::iterator iEst, iFinais;
	list<Transicao*>::iterator iTran;

	this->inicial = NULL;
	this->finais = list<Estado*>();

	for (iEst = this->estados.begin(); iEst != this->estados.end(); ++iEst)
		if (**iEst == *estadoInicial)
			this->inicial = *iEst;

	if (this->inicial == NULL)
		throw string("Erro na máquina [" + nome + "]: Estado inicial [" + estadoInicial->getNome() + "] inválido.\n");

	delete estadoInicial;

	for (iTran = this->transicoes.begin(); iTran != this->transicoes.end(); ++iTran) {
		if (!this->possuiEstado((*iTran)->getPartida()) || !this->possuiEstado((*iTran)->getChegada()))
			throw string("Erro na máquina [" + nome + "]: Transição [" + (*iTran)->toString() + "] inválida.\n");
	}

	for (iFinais = estadosFinais.begin(); iFinais != estadosFinais.end(); ++iFinais) {
		bool existe = false;

		for (iEst = this->estados.begin(); iEst != this->estados.end(); ++iEst)
			if (**iEst == **iFinais) {
				existe = true;
				(*iEst)->setFinal(true);
				this->finais.push_back(*iEst);
			}

		if (!existe) 
			throw string("Erro na máquina [" + nome + "]: Estado final [" + (*iFinais)->getNome() + "] inválido.\n");

		delete *iFinais;
	}

}

bool AFD::possuiEstado(Estado *e) {
	list<Estado*>::iterator i;

	for (i = this->estados.begin(); i != this->estados.end(); ++i)
		if ((**i) == *e)
			return true;

	return false;
}

Estado* AFD::realizaTransicao(Estado *e, string sim) {
	list<Transicao*>::iterator i;

	for (i = this->transicoes.begin(); i != this->transicoes.end(); ++i)
		if ((*i)->getPartida() == e && (*i)->getSimbolo() == sim)
			return (*i)->getChegada();

	return new Estado("");
}

void AFD::gerarDot() {
	ofstream arq;
	list<Estado*>::iterator iEst;
	list<Transicao*>::iterator iTran;

	arq.open((this->nome + ".dot").c_str(), ios::out);

	if (!arq.is_open())
		throw string("Erro ao gerar .dot da máquina [" + this->nome + "]\n");

	arq << "digraph \"" + this->nome + "\" {" << endl;
	arq << "\t_nil [style=\"invis\"];" << endl;
	arq << "\t_nil -> \"[" + this->inicial->getNome() + "]\" [label=\"\"];" << endl;

	for (iEst = this->finais.begin(); iEst != this->finais.end(); ++iEst)
		arq << "\t\"[" + (*iEst)->getNome() + "]\" [peripheries=2];" << endl;

	for (iTran = this->transicoes.begin(); iTran != this->transicoes.end(); ++iTran) {
		arq << "\t\"[" + (*iTran)->getPartida()->getNome() + "]\" -> \"[" + (*iTran)->getChegada()->getNome() + "]\" ";
		arq << "[label=" + (*iTran)->getSimbolo() + "];" << endl;
	}

	arq << "}" << endl;

	if (arq.is_open())
		arq.close();
}

string AFD::getNome() {
	return this->nome;
}