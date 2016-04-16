#include "transicao.h"

using namespace std;

Transicao::~Transicao() {
	delete this->partida;
	delete this->chegada;
}

Transicao::Transicao(Estado *p, std::string s, Estado *c) : partida(p), simbolo(s), chegada(c) {}

Estado* Transicao::getPartida() {
	return this->partida;
}

std::string Transicao::getSimbolo() {
	return this->simbolo;
}

Estado* Transicao::getChegada() {
	return this->chegada;
}

string Transicao::toString() {
	string transicao = "(";
	transicao += this->partida->getNome();
	transicao += ", " + this->simbolo + ") = ";
	transicao += this->chegada->getNome();

	return transicao;
}