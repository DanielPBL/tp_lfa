#ifndef __ALFABETO_H__
#define __ALFABETO_H__

#include <string>
#include <list>

class Alfabeto {
private:
	std::list<std::string> alfabeto;

public:
	Alfabeto();
	Alfabeto(std::list<std::string>);
	void adicionarSimbolo(std::string simbolo);
	bool pertence(std::string simbolo);
};

#endif // ifndef __ALFABETO_H__