#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <utility>
#include "algoritmoGenetico.h"

void mostrarSolucion(solucion mejorSol)
{
	std::vector<std::pair<int,double> > ruta=mejorSol.rutaSol;
	
	//Imprimir en consola el arreglo del cromosoma
	for(int i=0; i<ruta.size(); i++)
		std::cout<<"Ciudad "<<i+1<<": "<<ruta[i].first<<", peso: "<<ruta[i].second<<std::endl;
	
	//Imprimir en consola la aptitud del cromosoma
	std::cout<<"Aptitud: "<<mejorSol.aptituSol<<std::endl;
	
	//Imprimir en consola la generacion en la que se encontró la mejor solución
	std::cout<<"Generación: "<<mejorSol.generacionSol<<std::endl;
}

void ejecutarAlgoritmo(int ** matrAdy, int cantCiudades, int tamPoblacion, int cosMaximo, int totAristas, int cantGen)
{
	algoritmoGenetico* GA;
	GA=new algoritmoGenetico(matrAdy,cantCiudades,tamPoblacion,cosMaximo,totAristas,cantGen);
	GA->calcularSolucion();
	
	//Mostrar la solución encontrada
	mostrarSolucion(GA->getSolucion());
	
	delete GA;
}

void cargarValores(char * nombreIn, int tamPob, int costo, int numGen)
{
	int ** values;
	int numNodos, numAristas, tmpN1, tmpN2, tmpP;
	int aristaTot=0;
	
	//Llenar la matriz de adyacencia correspondiente al grafo del archivo
	std::ifstream file;
	file.open(nombreIn, std::ios::in);
	file>>numNodos;
	file>>numAristas;

	values=new int *[numNodos];
	for (int i=0; i<numNodos; i++)
	{
		values[i]=new int [numNodos];
		for(int j=0; j<numNodos; j++)
		{
			values[i][j]=-1;
		}
	}
	
	for(int idx=0; idx<numAristas; idx++)
	{
		file>>tmpN1;
		file>>tmpN2;
		file>>tmpP;

		aristaTot+=tmpP;
		
		values[tmpN1][tmpN2]=tmpP;
		values[tmpN2][tmpN1]=tmpP;
	}
	file.close();
	
	//Ejecutar el algoritmo genético
	ejecutarAlgoritmo(values,numNodos,tamPob,costo,aristaTot, numGen);
	
	//Eliminar la matriz de adyacencia utilizada
	for (int i=0; i<numNodos; i++)
	{
		delete [] values[i];
	}
	delete [] values;
	values=0;
}

int main(int argc, char **argv)
{
	int tamPob=100; //Valor por defecto
	int costo=-1;
	int gen=5000;
	std::string arg1;
	std::string arg2;
	
	switch (argc)
	{
		case 2:
		{
			cargarValores(argv[1], tamPob, costo, gen);
			break;
		}
		case 4:
		{
			arg1=argv[2];
			if(arg1.compare("-t")==0)
			{
				tamPob=atoi(argv[3]);
			}
			else if(arg1.compare("-c")==0)
			{
				costo=atoi(argv[3]);
			}
			else
			{
				gen=atoi(argv[3]);
			}
			cargarValores(argv[1], tamPob, costo, gen);
			break;
		}
		case 6:
		{
			arg1=argv[2];
			arg2=argv[4];
			if(arg1.compare("-t")==0)
			{
				if(arg2.compare("-c")==0)
				{
					tamPob=atoi(argv[3]);
					costo=atoi(argv[5]);
				}
				else
				{
					tamPob=atoi(argv[3]);
					gen=atoi(argv[5]);
				}
			}
			else if(arg1.compare("-c")==0)
			{
				if(arg2.compare("-t")==0)
				{
					tamPob=atoi(argv[5]);
					costo=atoi(argv[3]);
				}
				else
				{
					costo=atoi(argv[3]);
					gen=atoi(argv[5]);
				}
			}
			else
			{
				if(arg2.compare("-t")==0)
				{
					tamPob=atoi(argv[5]);
					gen=atoi(argv[3]);
				}
				else
				{
					gen=atoi(argv[3]);
					costo=atoi(argv[5]);
				}
			}
			cargarValores(argv[1], tamPob, costo, gen);
			break;
		}
		case 8:
		{
			arg1=argv[2];
			arg2=argv[4];
			if(arg1.compare("-t")==0)
			{
				if(arg2.compare("-c")==0)
				{
					tamPob=atoi(argv[3]);
					costo=atoi(argv[5]);
					gen=atoi(argv[7]);
				}
				else
				{
					tamPob=atoi(argv[3]);
					costo=atoi(argv[7]);
					gen=atoi(argv[5]);
				}
			}
			else if(arg1.compare("-c")==0)
			{
				if(arg2.compare("-t")==0)
				{
					tamPob=atoi(argv[5]);
					costo=atoi(argv[3]);
					gen=atoi(argv[7]);
				}
				else
				{
					tamPob=atoi(argv[7]);
					costo=atoi(argv[3]);
					gen=atoi(argv[5]);
				}
			}
			else
			{
				if(arg2.compare("-t")==0)
				{
					tamPob=atoi(argv[5]);
					costo=atoi(argv[7]);
					gen=atoi(argv[3]);
				}
				else
				{
					tamPob=atoi(argv[7]);
					costo=atoi(argv[5]);
					gen=atoi(argv[3]);
				}
			}
			cargarValores(argv[1], tamPob, costo, gen);
			break;
		}
		default:
		{
			std::cout<<"main: archivoGrafo -t tamañoPoblacion -c costo -g generaciones"<<std::endl;
			break;
		}
	}
}
