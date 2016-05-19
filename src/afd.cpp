#include "afd.h"
#include "globals.h"
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

AFD::AFD(string nome, Estado *inicial) : nome(nome), inicial(inicial) {
	this->estados = list<Estado*>();
	this->finais = list<Estado*>();
	this->adicionaEstado(inicial);
	this->transicoes = list<Transicao*>();
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

		if (existe)
			for (iTran = this->transicoes.begin(); iTran != this->transicoes.end(); ++iTran) {
				if (*((*iTran)->getPartida()) == **iFinais)
					((*iTran)->getPartida())->setFinal(true);
				if (*((*iTran)->getChegada()) == **iFinais)
					((*iTran)->getChegada())->setFinal(true);
			}

		if (!existe) 
			throw string("Erro na máquina [" + nome + "]: Estado final [" + (*iFinais)->getNome() + "] inválido.\n");

		delete *iFinais;
	}

}

void AFD::adicionaEstado(Estado *estado) {
	if (!this->possuiEstado(estado)) {
		this->estados.push_back(estado);
		if (estado->isFinal())
			this->finais.push_back(estado);
	}
}

void AFD::adicionaTransicao(Transicao *Transicao) {
	this->transicoes.push_back(Transicao);
}

bool AFD::possuiEstado(Estado *e) {
	list<Estado*>::iterator i;

	for (i = this->estados.begin(); i != this->estados.end(); ++i)
		if (**i == *e)
			return true;

	return false;
}

Estado* AFD::realizaTransicao(Estado *e, string sim) {
	list<Transicao*>::iterator i;

	for (i = this->transicoes.begin(); i != this->transicoes.end(); ++i)
		if (*((*i)->getPartida()) == *e && (*i)->getSimbolo() == sim)
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

void AFD::setNome(string nome) {
	this->nome = nome;
}

string AFD::getNome() {
	return this->nome;
}

Estado* AFD::getInicial() {
	return this->inicial;
}

std::list<Estado*> AFD::getEstados() {
	return this->estados;
}

std::list<Transicao*> AFD::getTransicoes() {
	return this->transicoes;
}

AFD* AFD::produto(AFD* m1, AFD* m2, Operacao op) {
	list<string> alfabeto = global::alfabeto.getAlfabeto();
	list<string>::iterator alf;

	EstadoComposto *inicial = new EstadoComposto();
	inicial->e1 = m1->getInicial();
	inicial->e2 = m2->getInicial();
	inicial->comp = new Estado(m1->getInicial()->getNome() + "," + m2->getInicial()->getNome());

	if (op == INTERSECAO && inicial->e1->isFinal() && inicial->e2->isFinal())
		inicial->comp->setFinal(true);
	else if (op == UNIAO && (inicial->e1->isFinal() || inicial->e2->isFinal()))
		inicial->comp->setFinal(true);

	AFD *produto;
	if (op == INTERSECAO)
		produto = new AFD("intersecao", inicial->comp);
	else if (op == UNIAO)
		produto = new AFD("uniao", inicial->comp);

	list<EstadoComposto*> novosEstados = list<EstadoComposto*>();
	novosEstados.push_back(inicial);
	list<EstadoComposto*>::iterator it;

	for (it = novosEstados.begin(); it != novosEstados.end(); ++it) {
		EstadoComposto *ec = *it;

		for (alf = alfabeto.begin(); alf != alfabeto.end(); ++alf) {
			Estado *eM1 = m1->realizaTransicao(ec->e1, *alf), *eM2 = m2->realizaTransicao(ec->e2, *alf);
			EstadoComposto *novoEstado = new EstadoComposto();

			novoEstado->e1 = eM1;
			novoEstado->e2 = eM2;
			novoEstado->comp = new Estado(eM1->getNome() + "," + eM2->getNome());

			if (novoEstado->comp->getNome() == ",") {
				if (!global::completo)
					continue;
				else {
					delete novoEstado->comp;
					novoEstado->comp = new Estado("e'");
				}
			}

			if (op == INTERSECAO && novoEstado->e1->isFinal() && novoEstado->e2->isFinal())
				novoEstado->comp->setFinal(true);
			else if (op == UNIAO && (novoEstado->e1->isFinal() || novoEstado->e2->isFinal()))
				novoEstado->comp->setFinal(true);

			if (!produto->possuiEstado(novoEstado->comp)) {				
				produto->adicionaEstado(novoEstado->comp);
				novosEstados.push_back(novoEstado);
			}

			produto->adicionaTransicao(new Transicao(ec->comp, *alf, novoEstado->comp));
		}
	}

	return produto;
}

AFD* AFD::intersecao(AFD* m1, AFD* m2) { 
	return AFD::produto(m1, m2, INTERSECAO);
}

AFD* AFD::uniao(AFD *m1, AFD *m2) {
	return AFD::produto(m1, m2, UNIAO);
}