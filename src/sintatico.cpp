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
	this->procAlfabeto();
	this->procMaquinas();
	this->matchToken(FIM_ARQ_NORMAL);
}

void AnalisadorSintatico::procAlfabeto() {
	if (this->atual.token != "A") {
		this->matchToken(GERADOR_EXCESSOES);
		return;
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);
	this->procSimbolos();
	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);
}

void AnalisadorSintatico::procSimbolos() {
	this->matchToken(SIMBOLO);

	if (this->atual.tipo == VIRGULA) {
		this->matchToken(VIRGULA);
		this->procSimbolos();
	}
}

void AnalisadorSintatico::procNome() {
	this->matchToken(SIMBOLO);

	if (this->atual.tipo == SIMBOLO)
		this->procNome();
}

void AnalisadorSintatico::procMaquina() {
	this->procNome();
	this->matchToken(ABRE_CHAVES);
	this->procEstadosM();
	this->procTransicoesM();
	this->procInicial();
	this->procFinais();
	this->matchToken(FECHA_CHAVES);
}

void AnalisadorSintatico::procMaquinas() {
	this->procMaquina();

	if (this->atual.tipo == SIMBOLO)
		procMaquinas();
}

void AnalisadorSintatico::procEstadosM() {
	if (this->atual.token != "E") {
		this->matchToken(GERADOR_EXCESSOES);
		return;
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);
	this->procEstados();
	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);
}

void AnalisadorSintatico::procEstados() {
	this->procEstado();

	if (this->atual.tipo == VIRGULA) {
		this->matchToken(VIRGULA);
		this->procEstados();
	}
}

void AnalisadorSintatico::procEstado() {
	this->procNome();
}

void AnalisadorSintatico::procTransicoesM() {
	if (this->atual.token != "T") {
		this->matchToken(GERADOR_EXCESSOES);
		return;
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);
	this->procTransicoes();
	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);
}

void AnalisadorSintatico::procTransicoes() {
	this->procTransicao();

	if (this->atual.tipo == VIRGULA) {
		this->matchToken(VIRGULA);
		this->procTransicoes();
	}
}

void AnalisadorSintatico::procTransicao() {
	this->matchToken(ABRE_PARENTESES);
	this->procEstado();
	this->matchToken(VIRGULA);
	this->matchToken(SIMBOLO);
	this->matchToken(FECHA_PARENTESES);
	this->matchToken(IGUAL);
	this->procEstado();
}

void AnalisadorSintatico::procInicial() {
	if (this->atual.token != "i") {
		this->matchToken(GERADOR_EXCESSOES);
		return;
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->procEstado();
	this->matchToken(PONTO_VIRGULA);
}

void AnalisadorSintatico::procFinais() {
	if (this->atual.token != "F") {
		this->matchToken(GERADOR_EXCESSOES);
		return;
	}

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);
	this->procEstados();
	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);	
}