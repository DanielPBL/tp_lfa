#ifndef __TRANSICAO_H__
#define __TRANSICAO_H__

#include <string>
#include "estado.h"

class Transicao {
private:
	Estado *partida;
	std::string simbolo;
	Estado *chegada;

public:
	Transicao(Estado*, std::string, Estado*);
	~Transicao();
	Estado* getPartida();
	std::string getSimbolo();
	Estado* getChegada();
	std::string toString();
};

#endif // ifndef __TRANSICAO_H__