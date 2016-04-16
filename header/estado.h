#ifndef __ESTADO_H__
#define __ESTADO_H__

#include <string>

class Estado {
private:
	std::string nome;
	bool final;

public:
	Estado(std::string);
	Estado(std::string, bool);
	friend bool operator == (const Estado&, const Estado&);
	bool isFinal();
	void setFinal(bool);
	std::string getNome() const;
};

#endif // ifndef __ESTADO_H__