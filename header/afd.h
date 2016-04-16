#ifndef __AFD_H__
#define __AFD_H__

#include "estado.h"
#include "transicao.h"
#include <string>
#include <list>

class AFD {
private:
	std::string nome;
	std::list<Estado*> estados;
	std::list<Transicao*> transicoes;
	Estado *inicial;
	std::list<Estado*> finais;

public:
	AFD(std::string, std::list<Estado*>, std::list<Transicao*>, Estado*, std::list<Estado*>);
	~AFD();
	bool possuiEstado(Estado*);
	Estado* realizaTransicao(Estado*, std::string);
	void gerarDot();
	std::string getNome();
};

#endif // ifndef __AFD_H__