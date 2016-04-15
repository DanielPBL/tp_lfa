#ifndef __SINTATICO_H__
#define __SINTATICO_H__

#include "tp.h"

class AnalisadorSintatico {
private:
	AnalisadorLexico& lexica;
	Lexema atual;

public:
	AnalisadorSintatico(AnalisadorLexico& lexica);
	void matchToken(int);
	void init();

private:

	void lancaExcessao(std::string);
	void procSimbolo();
	void procNome();
	void procEstado();
	void procEstados();
	void procTransicao();
	void procTransicoes();
	void procSimbolos();
	void procAlfabeto();
	void procEstadosM();
	void procTransicoesM();
	void procInicial();
	void procFinais();
	void procMaquina();
	void procMaquinas();
	void procPrograma();
};

#endif // ifndef __SINTATICO_H__
