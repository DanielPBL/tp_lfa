#include "alfabeto.h"

using namespace std;

Alfabeto::Alfabeto() {
	this->alfabeto = list<string>();
}

Alfabeto::Alfabeto(list<string> alfabeto) : alfabeto(alfabeto) {}

void Alfabeto::adicionarSimbolo(string simbolo) {
	this->alfabeto.push_back(simbolo);
}

bool Alfabeto::pertence(string simbolo) {
	list<string>::iterator i;

	for (i = this->alfabeto.begin(); i != this->alfabeto.end(); ++i)
		if (*i == simbolo)
			return true;

	return false;
}

list<string> Alfabeto::getSimbolos() {
	return this->alfabeto;
}