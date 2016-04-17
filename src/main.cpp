#include <iostream>
#include <string>
#include "tp.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

using namespace std;

int main(int argc, char **argv) {
	if (argc < 2) {
		cout << "Usar: gerador [AFD's de entrada] [-c]" << endl;
		return EXIT_FAILURE;
	}

	if (argc == 3 && string(argv[2]) == "-c")
		global::completo = true;
	else if (argc >= 3) {
		cout << "Usar: gerador [AFD's de entrada] [-c]" << endl;
		return EXIT_FAILURE;
	}

	try {
		AnalisadorLexico lexico(argv[1]);
		AnalisadorSintatico sintatico(lexico);
		sintatico.init();
	} catch (string msg) {
		cout << msg << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}