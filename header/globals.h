#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "alfabeto.h"
#include "afd.h"
#include <list>

namespace global {
	extern Alfabeto alfabeto;
	extern std::list<AFD*> maquinas;
}

#endif // ifndef __GLOBALS_H__