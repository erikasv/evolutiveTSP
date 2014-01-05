#include "random_generator.h"

random_generator::random_generator()
{
	semilla = time(NULL);
	a = 16807;
	m = 2147483647;
}

random_generator::~random_generator()
{
}

double random_generator::generarNumero()
{
	semilla =  fmod((a * semilla), m);
	return (double) (semilla/m);
}
