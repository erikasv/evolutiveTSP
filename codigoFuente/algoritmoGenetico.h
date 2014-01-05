/*
 * Erika Suárez Valencia - 0743588
 * Nathaly Mesa García - 0750960
 */
#ifndef ALGORITMOGENETICO_H
#define ALGORITMOGENETICO_H

#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>
#include <utility>
#include <algorithm>
#include "cromosoma.h"
#include "random_generator.h"

//Estructura para ordenar las ciudades de acuerdo al peso de cada una.
extern struct orden {
	bool operator() (std::pair<int, double> ciudad1, std::pair<int, double> ciudad2)
	{
		return (ciudad1.second < ciudad2.second);
	}
} miOrden;

//Clase en la que se entrega la solución encontrada por el algoritmo.
class solucion {
public:
	int aptituSol;
	int generacionSol;
	std::vector<std::pair<int, double> > rutaSol;
};

class algoritmoGenetico
{
private:
	int** grafo; //El grafo correspondiente a las ciudades y sus conexiones
	int cantidadCiudades; //La cantidad de ciudades = canditad de nodos = tamaño del cromosoma
	int tamanoPoblacion; //El tamaño de la población
	int costoMaximo; //Costo máximo con el que se desea encontrar una solución, si es -1 se busca el mínimo posible
	int totalAristas; //El peso total de los caminos entre las ciudades (usado para penalizar los cromosomas)
	int cantGeneraciones; //La cantidad máxima de generaciones que se dejará correr el algorimo
	random_generator generador;
	
	cromosoma* optimo; //Almacena el mejor en cada generación
	int generacion; //Generaciones transcurridas
	std::vector<cromosoma*> poblacion; //Población de cromosomas de la generación actual
	std::vector<cromosoma*> mattingPool; //Cromosomas seleccionados, que producen la siguiente generación
	
	std::vector<int> ultimosMejores; //Los menores costos de las ultimas n generaciones donde n = tamaño de la poblacion
	
	void poblacionInicial();
	void calcularAptitudPoblacion();
	int calcularAptitudCromosoma(int idx);
	bool hayOptimo();
	void nuevaGeneracion();
	void seleccionarCromosomas();
	void cruzarSeleccion();
	void generarHijo(int padre, int madre);
	void mutarSeleccion();
	
public:
	algoritmoGenetico(int** grafoIN, int cantidadCiudadesIN, int tamanoPoblacionIN, int costoMaximoIN, int totalAristasIN, int cantGeneracionesIN);
	~algoritmoGenetico();
	void calcularSolucion();
	solucion getSolucion();
};

#endif