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
	std::list<Estado*> procEstadosExt();
	std::list<Transicao*> procTransicoesExt();
	std::list<std::string> procSimbolosExt();
	AFD* procMaquina();
	void procMaquinas();
	void procPrograma();
};

#endif // ifndef __SINTATICO_H__
