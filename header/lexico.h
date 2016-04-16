#ifndef __LEXICO_H__
#define __LEXICO_H__

#include <fstream>
#include <string>
#include <map>

#define FIM_ARQ_INESPERADO -2
#define TOKEN_INVALIDO     -1
#define FIM_ARQ_NORMAL     0
#define SIMBOLO            1
#define SETA               2
#define ABRE_CHAVES        3
#define VIRGULA            4
#define FECHA_CHAVES       5
#define PONTO_VIRGULA      6
#define ABRE_PARENTESES    7
#define FECHA_PARENTESES   8
#define IGUAL              9


typedef struct LEXEMA {
	std::string token;
	int tipo;
} Lexema;

class AnalisadorLexico {
private:
	std::ifstream arquivo;
	int linha;
	std::map<std::string, int> tabelaSimbolos;

public:
	AnalisadorLexico(char *arquivo);
	~AnalisadorLexico();
	Lexema getLexema();
	int getLinha();

private:
	int consultarTabelaSimbolos(std::string);
	void criarTabelaSimbolos();
	void ungetChar(char);
	static bool isDigit(char);
	static bool isLowerLetter(char);
	static bool isUperLetter(char);
	static bool isLetter(char);
	static bool isBlank(char);
	static bool isSimbolo(char);
};

#endif // ifndef __LEXICO_H__
