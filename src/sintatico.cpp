#include "sintatico.h"
#include <cstdio>
#include <cstdlib>
#include <string>

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
	list<AFD*>::iterator it;

	global::maquinas  = list<AFD*>();
	global::alfabeto = this->procAlfabeto();

	global::maquinas.push_back(this->procMaquina());

	this->procMaquinas();

	this->matchToken(FIM_ARQ_NORMAL);

	for (it = global::maquinas.begin(); it != global::maquinas.end(); ++it) 
		(*it)->gerarDot(false);

	AFD *intersecao, *uniao;

	it = global::maquinas.begin();
	intersecao = AFD::intersecao(*(global::maquinas.begin()), *(++it));
	it = global::maquinas.begin();
	uniao = AFD::uniao(*(global::maquinas.begin()), *(++it));

	for (++it; it != global::maquinas.end(); ++it) {
		intersecao = AFD::intersecao(intersecao, *it);
		uniao = AFD::uniao(uniao, *it);
	}

	intersecao->gerarDot(true);
	uniao->gerarDot(true);

	for (it = global::maquinas.begin(); it != global::maquinas.end(); ++it) 
		delete *it;
}

Alfabeto AnalisadorSintatico::procAlfabeto() {
	if (this->atual.token != "A")
		this->lancaExcessao("Lexema não esperado [" + this->atual.token + "]");

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);

	Alfabeto alfabeto = Alfabeto(this->procSimbolosExt());

	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);

	return alfabeto;
}

list<string> AnalisadorSintatico::procSimbolosExt() {
	list<string> simbolos = list<string>();

	if (this->atual.tipo != SIMBOLO)
		return simbolos;

	return this->procSimbolos();
}

list<string> AnalisadorSintatico::procSimbolos() {
	list<string> simbolos = list<string>();
	simbolos.push_back(this->atual.token);

	this->matchToken(SIMBOLO);

	if (this->atual.tipo == VIRGULA) {
		this->matchToken(VIRGULA);
		list<string> novosSimbolos = this->procSimbolos();
		simbolos.splice(simbolos.end(), novosSimbolos);
	}

	return simbolos;
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

	list<AFD*>::iterator it;

	for (it = global::maquinas.begin(); it != global::maquinas.end(); ++it)
		if ((*it)->getNome() == nome)
			this->lancaExcessao("Dupla declaração para a máquina [" + nome + "]");

	this->matchToken(ABRE_CHAVES);

	list<Estado*> estados = this->procEstadosM();
	list<Transicao*> transicoes = this->procTransicoesM();
	Estado *estadoInicial = this->procInicial();

	list<Estado*> estadosFinais = this->procFinais();
	this->matchToken(FECHA_CHAVES);

	return new AFD(nome, estados, transicoes, estadoInicial, estadosFinais);
}

void AnalisadorSintatico::procMaquinas() {
	global::maquinas.push_back(this->procMaquina());

	if (this->atual.tipo == SIMBOLO)
		this->procMaquinas();
}

list<Estado*> AnalisadorSintatico::procEstadosM() {
	if (this->atual.token != "E")
		this->lancaExcessao("Lexema não esperado [" + this->atual.token + "]");

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);

	list<Estado*> estados = this->procEstadosExt();

	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);

	return estados;
}

list<Estado*> AnalisadorSintatico::procEstadosExt() {
	list<Estado*> estados = list<Estado*>();

	if (this->atual.tipo != SIMBOLO)
		return estados;

	return this->procEstados();
}

list<Estado*> AnalisadorSintatico::procEstados() {
	list<Estado*> estados = list<Estado*>();
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
	if (this->atual.token != "T")
		this->lancaExcessao("Lexema não esperado [" + this->atual.token + "]");

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);

	list<Transicao*> transicoes = this->procTransicoesExt();

	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);

	return transicoes;
}

list<Transicao*> AnalisadorSintatico::procTransicoesExt() {
	list<Transicao*> transicoes = list<Transicao*>();

	if (this->atual.tipo != ABRE_PARENTESES)
		return transicoes;

	return this->procTransicoes();
}

list<Transicao*> AnalisadorSintatico::procTransicoes() {
	list<Transicao*> transicoes = list<Transicao*>();
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
	if (this->atual.token != "i")
		this->lancaExcessao("Lexema não esperado [" + this->atual.token + "]");

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	Estado *estadoInicial = this->procEstado();
	this->matchToken(PONTO_VIRGULA);

	return estadoInicial;
}

list<Estado*> AnalisadorSintatico::procFinais() {
	if (this->atual.token != "F")
		this->lancaExcessao("Lexema não esperado [" + this->atual.token + "]");

	this->matchToken(SIMBOLO);
	this->matchToken(SETA);
	this->matchToken(ABRE_CHAVES);
	list<Estado*> estadosFinais = this->procEstadosExt();
	this->matchToken(FECHA_CHAVES);
	this->matchToken(PONTO_VIRGULA);

	return estadosFinais;
}
