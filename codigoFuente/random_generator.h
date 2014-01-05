#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <time.h>
#include <cmath>

class random_generator
{
private:
	double semilla;
	double a;
	double m;

public:
	random_generator();
	~random_generator();
	double generarNumero();
};
#endif
