/*
 * Erika Suárez Valencia - 0743588
 * Nathaly Mesa García - 0750960
 */
#ifndef CROMOSOMA_H
#define CROMOSOMA_H

#include <vector>
#include "random_generator.h"

class cromosoma
{
private:
	std::vector<double> arregloCromosoma; //Las ciudades que hay que visitar
	int aptitud; //costo de visitarlas en el orden dado
	random_generator generador;
	
public:
	cromosoma(std::vector<double> arregloCromosomaIN);
	cromosoma(cromosoma*& copia);
	~cromosoma();
	
	void mutarGen(int gen); //Cambiar el gen indicado añadiendole ruido
	double getGen(int idx);
	int getAptitud();
	void setAptitud(int aptitudIN);
};

#endif