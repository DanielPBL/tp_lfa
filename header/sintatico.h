#ifndef __SINTATICO_H__
#define __SINTATICO_H__

#include "tp.h"
#include <list>
#include <string>

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
	std::string procNome();
	Estado* procEstado();
	std::list<Estado*> procEstados();
	Transicao* procTransicao();
	std::list<Transicao*> procTransicoes();
	std::list<std::string> procSimbolos();
	Alfabeto procAlfabeto();
	std::list<Estado*> procEstadosM();
	std::list<Transicao*> procTransicoesM();
	Estado* procInicial();
	std::list<Estado*> procFinais();
	AFD* procMaquina();
	std::list<AFD*> procMaquinas();
	void procPrograma();
};

#endif // ifndef __SINTATICO_H__
