#include "lexico.h"
#include <string>

using namespace std;

AnalisadorLexico::AnalisadorLexico(char *arquivo) {
	this->arquivo.open(arquivo);
	this->linha = 1;
	this->criarTabelaSimbolos();

	if (!this->arquivo.is_open())
		throw string("Erro ao abrir o arquivo!\n");
}

AnalisadorLexico::~AnalisadorLexico() {
	if (this->arquivo.is_open())
		this->arquivo.close();
}

int AnalisadorLexico::getLinha() {
	return this->linha;
}

bool AnalisadorLexico::isDigit(char c) {
	int ch = (int)c;

	return ch >= 48 && ch <= 57;
}

bool AnalisadorLexico::isLowerLetter(char c) {
	int ch = (int)c;

	return ch >= 97 && ch <= 122;
}

bool AnalisadorLexico::isUperLetter(char c) {
	int ch = (int)c;

	return ch >= 65 && ch <= 90;
}

bool AnalisadorLexico::isLetter(char c) {
	int ch = (int)c;

	return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122);
}

bool AnalisadorLexico::isBlank(char ch) {
	return ch == '\r' || ch == '\n' || ch == '\t' || ch == ' ';
}

bool AnalisadorLexico::isSimbolo(char ch) {
	return ch == '{' || ch == ',' || ch == '}' || ch == ';' || ch == '(' || ch == ')' || ch == '=';
}

int AnalisadorLexico::consultarTabelaSimbolos(string token) {
	if (this->tabelaSimbolos.find(token) != this->tabelaSimbolos.end())
		return this->tabelaSimbolos[token];

	return TOKEN_INVALIDO;
}

void AnalisadorLexico::criarTabelaSimbolos() {
	this->tabelaSimbolos["->"] = SETA;
	this->tabelaSimbolos["{"] = ABRE_CHAVES;
	this->tabelaSimbolos[","] = VIRGULA;
	this->tabelaSimbolos["}"] = FECHA_CHAVES;
	this->tabelaSimbolos[";"] = PONTO_VIRGULA;
	this->tabelaSimbolos["("] = ABRE_PARENTESES;
	this->tabelaSimbolos[")"] = FECHA_PARENTESES;
	this->tabelaSimbolos["="] = IGUAL;
}

void AnalisadorLexico::ungetChar(char ch) {
	if (ch == '\n') {
		this->linha--;
	}
	this->arquivo.unget();
}

Lexema AnalisadorLexico::getLexema() {
	Lexema lexema;
	char   ch = -1;

	lexema.tipo  = FIM_ARQ_NORMAL;
	lexema.token = "";
	int estado = 1;

	while (estado != 5) {
		this->arquivo >> noskipws >> ch;

		if (ch == '\n') this->linha++;

		if ((this->arquivo.eof() || this->arquivo.fail())) {
			if (estado != 1)
				lexema.tipo = FIM_ARQ_INESPERADO;

			break;
		}

		switch (estado) {
			case 1:
				if (AnalisadorLexico::isBlank(ch)) {
					continue;
				} else if (AnalisadorLexico::isSimbolo(ch)) {
					lexema.token += ch;
					lexema.tipo = this->consultarTabelaSimbolos(lexema.token);
					estado = 5;
				} else if (ch == '-') {
					lexema.token += ch;
					estado = 2;
				} else if (AnalisadorLexico::isLetter(ch) || AnalisadorLexico::isDigit(ch)) {
					lexema.token += ch;
					lexema.tipo = SIMBOLO;
					estado = 5;
				} else if (ch == '/') {
					estado = 3;
				} else {
					lexema.token += ch;
					lexema.tipo = TOKEN_INVALIDO;
					estado = 5;
				}
				break;

			case 2:
				if (ch == '>') {
					lexema.token += ch;
					lexema.tipo   = this->consultarTabelaSimbolos(lexema.token);
				} else {
					lexema.tipo = this->consultarTabelaSimbolos(lexema.token);
					this->ungetChar(ch);
				}
				estado = 5;
				break;

			case 3:
				if (ch == '/') {
					estado = 4;
				} else {
					lexema.token += ch;
					lexema.tipo = this->consultarTabelaSimbolos(lexema.token);
					estado = 5;
				}
				break;

			case 4:
				if (ch == '\n')
					estado = 1;
		}
	}
	return lexema;
}
