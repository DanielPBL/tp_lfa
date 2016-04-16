#ifndef __AFD_H__
#define __AFD_H__

#include "estado.h"
#include "transicao.h"
#include <string>
#include <list>

typedef struct ESTADO_COMPOSTO {
	Estado *e1;
	Estado *e2;
	Estado *comp;
} EstadoComposto;

enum Operacao { INTERSECAO, UNIAO };

class AFD {
private:
	std::string nome;
	std::list<Estado*> estados;
	std::list<Transicao*> transicoes;
	Estado *inicial;
	std::list<Estado*> finais;

public:
	AFD(std::string, Estado*);
	AFD(std::string, std::list<Estado*>, std::list<Transicao*>, Estado*, std::list<Estado*>);
	~AFD();
	bool possuiEstado(Estado*);
	void adicionaEstado(Estado*);
	void adicionaTransicao(Transicao*);
	Estado* realizaTransicao(Estado*, std::string);
	void gerarDot();
	void setNome(std::string);
	std::string getNome();
	std::list<Estado*> getEstados();
	std::list<Transicao*> getTransicoes();
	Estado* getInicial();
	static AFD* intersecao(AFD*, AFD*);
	static AFD* uniao(AFD*, AFD*);

private:
	static AFD* produto(AFD*, AFD*, Operacao);
};

#endif // ifndef __AFD_H__