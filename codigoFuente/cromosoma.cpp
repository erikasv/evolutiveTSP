/*
 * Erika Suárez Valencia - 0743588
 * Nathaly Mesa García - 0750960
 */
#include "cromosoma.h"

//Constructor
cromosoma::cromosoma(std::vector<double> arregloCromosomaIN)
{
	arregloCromosoma=std::vector<double>(arregloCromosomaIN.size());
	for(int i=0; i<arregloCromosomaIN.size(); i++)
		arregloCromosoma[i]=arregloCromosomaIN[i];
	
	aptitud=0;
	
	generador=random_generator();
}

//Constructor de copia
cromosoma::cromosoma(cromosoma*& copia)
{
	arregloCromosoma=copia->arregloCromosoma;
	aptitud=copia->aptitud;
	generador=random_generator();
}

//Destructor
cromosoma::~cromosoma()
{
	arregloCromosoma.clear();
}

//Cambiar el gen indicado añadiendole ruido
void cromosoma::mutarGen(int gen)
{
	arregloCromosoma[gen]+= generador.generarNumero()*2 - 1;
}

//Retornar el valor del gen indicado
double cromosoma::getGen(int idx)
{
	return arregloCromosoma[idx];
}

//Retornar el valor de la aptitud del cromosoma
int cromosoma::getAptitud()
{
	return aptitud;
}

//Cambiar el valor de la aptitud del cromosoma
void cromosoma::setAptitud(int aptitudIN)
{
	aptitud=aptitudIN;
}