#include "sintatico.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

AnalisadorSintatico::AnalisadorSintatico(AnalisadorLexico& lexica) : lexica(lexica) {
	this->atual = lexica.getLexema();
}

void AnalisadorSintatico::matchToken(int tipo) {
	if (this->atual.tipo == tipo) {
		this->atual = this->lexica.getLexema();
	} else {
		switch (this->atual.tipo) {
		case TOKEN_INVALIDO:
			this->lancaExcessao("Lexema inválido [" + this->atual.token + "]");
			break;

		case FIM_ARQ_INESPERADO:
		case FIM_ARQ_NORMAL:
			this->lancaExcessao("Fim de arquivo inesperado");
			break;

		default:
			this->lancaExcessao("Lexema não esperado [" + this->atual.token + "]");
		}
	}
}


void AnalisadorSintatico::lancaExcessao(string aviso) {
	char   l[10];
	string msg = "";

	sprintf(l, "%02d", this->lexica.getLinha());
	string linha(l);
	msg = linha + ": " + aviso;

	throw msg;
}


void AnalisadorSintatico::init() {
	this->procPrograma();
}

void AnalisadorSintatico::procPrograma() {
	list<AFD*> maquinas = list<AFD*>(), novasMaquinas;
	list<AFD*>::iterator it;

	global::alfabeto = this->procAlfabeto();

	maquinas.push_back(this->procMaquina());

	novasMaquinas = this->procMaquinas();
	maquinas.splice(maquinas.end(), novasMaquinas);

	for (it = maquinas.begin(); it != maquinas.end(); ++it) {
		list<AFD*>::iterator itInt;
		for (itInt = maquinas.begin(); itInt != maquinas.end(); ++itInt)
			if ((*it)->getNome() == (*itInt)->getNome() && *it != *itInt)
				throw string("Dupla declaração para a máquina [" + (*it)->getNome() + "]");
	}

	this->matchToken(FIM_ARQ_NORMAL);

	for (it = maquinas.begin(); it != maquinas.end(); ++it) {
		(*it)->gerarDot();
		delete *it;
	}
}

Alfabeto AnalisadorSintatico::procAlfabeto() {
	if (this->atual.token != "A") {
		this->matchToken(GERADOR_EXCESSOES);
		return Alfabeto();
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);

	Alfabeto alfabeto = Alfabeto(this->procSimbolos());

	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);

	return alfabeto;
}

list<string> AnalisadorSintatico::procSimbolos() {
	list<string> alfabeto = list<string>();

	if (this->atual.tipo != SIMBOLO)
		return alfabeto;

	alfabeto.push_back(this->atual.token);

	this->matchToken(SIMBOLO);

	if (this->atual.tipo == VIRGULA) {
		this->matchToken(VIRGULA);
		list<string> novosSimbolos = this->procSimbolos();
		alfabeto.splice(alfabeto.end(), novosSimbolos);
	}

	return alfabeto;
}

string AnalisadorSintatico::procNome() {
	string nome = "";

	nome += this->atual.token;

	this->matchToken(SIMBOLO);

	if (this->atual.tipo == SIMBOLO)
		nome += this->procNome();

	return nome;
}

AFD* AnalisadorSintatico::procMaquina() {
	string nome = this->procNome();
	this->matchToken(ABRE_CHAVES);

	list<Estado*> estados = this->procEstadosM();
	list<Transicao*> transicoes = this->procTransicoesM();
	Estado *estadoInicial = this->procInicial();

	list<Estado*> estadosFinais = this->procFinais();
	this->matchToken(FECHA_CHAVES);

	return new AFD(nome, estados, transicoes, estadoInicial, estadosFinais);
}

list<AFD*> AnalisadorSintatico::procMaquinas() {
	list<AFD*> maquinas = list<AFD*>();

	maquinas.push_back(this->procMaquina());

	if (this->atual.tipo == SIMBOLO) {
		list<AFD*> novasMaquinas = this->procMaquinas();
		maquinas.splice(maquinas.end(), novasMaquinas);
	}

	return maquinas;
}

list<Estado*> AnalisadorSintatico::procEstadosM() {
	if (this->atual.token != "E") {
		this->matchToken(GERADOR_EXCESSOES);
		return list<Estado*>();
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);

	list<Estado*> estados = this->procEstados();

	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);

	return estados;
}

list<Estado*> AnalisadorSintatico::procEstados() {
	list<Estado*> estados = list<Estado*>();

	if (this->atual.tipo != SIMBOLO)
		return estados;

	estados.push_back(this->procEstado());

	if (this->atual.tipo == VIRGULA) {
		this->matchToken(VIRGULA);
		list<Estado*> novosEstados = this->procEstados();
		estados.splice(estados.end(), novosEstados);
	}

	return estados;
}

Estado* AnalisadorSintatico::procEstado() {
	return new Estado(this->procNome());
}

list<Transicao*> AnalisadorSintatico::procTransicoesM() {
	if (this->atual.token != "T") {
		this->matchToken(GERADOR_EXCESSOES);
		return list<Transicao*>();
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);

	list<Transicao*> transicoes = this->procTransicoes();

	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);

	return transicoes;
}

list<Transicao*> AnalisadorSintatico::procTransicoes() {
	list<Transicao*> transicoes = list<Transicao*>();

	if (this->atual.tipo != ABRE_PARENTESES)
		return transicoes;

	transicoes.push_back(this->procTransicao());

	if (this->atual.tipo == VIRGULA) {
		this->matchToken(VIRGULA);
		list<Transicao*> novasTransicoes = this->procTransicoes();
		transicoes.splice(transicoes.end(), novasTransicoes);
	}

	return transicoes;
}

Transicao* AnalisadorSintatico::procTransicao() {
	this->matchToken(ABRE_PARENTESES);
	Estado *partida = this->procEstado();
	this->matchToken(VIRGULA);

	string simbolo = this->atual.token;
	this->matchToken(SIMBOLO);

	if (!global::alfabeto.pertence(simbolo))
		this->lancaExcessao("Simbolo [" + simbolo + "] não pertence ao alfabeto (A)");

	this->matchToken(FECHA_PARENTESES);
	this->matchToken(IGUAL);
	Estado *chegada = this->procEstado();

	return new Transicao(partida, simbolo, chegada);
}

Estado* AnalisadorSintatico::procInicial() {
	if (this->atual.token != "i") {
		this->matchToken(GERADOR_EXCESSOES);
		return new Estado("");
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	Estado *estadoInicial = this->procEstado();
	this->matchToken(PONTO_VIRGULA);

	return estadoInicial;
}

list<Estado*> AnalisadorSintatico::procFinais() {
	if (this->atual.token != "F") {
		this->matchToken(GERADOR_EXCESSOES);
		return list<Estado*>();
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);
	list<Estado*> estadosFinais = this->procEstados();
	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);

	return estadosFinais;
}