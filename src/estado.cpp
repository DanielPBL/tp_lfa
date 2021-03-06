#include "estado.h"

using namespace std;

Estado::Estado(string nome) : nome(nome) {
	final = false;
}

Estado::Estado(string nome, bool final) : nome(nome), final(final) {}

bool operator == (const Estado& e1, const Estado& e2) {
	return (e1.getNome() == e2.getNome());
}

bool Estado::isFinal() {
	return this->final;
}

void Estado::setFinal(bool final) {
	this->final = final;
}

string Estado::getNome() const {
	return this->nome;
}