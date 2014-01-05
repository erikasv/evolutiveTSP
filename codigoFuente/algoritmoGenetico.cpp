/*
 * Erika Suárez Valencia - 0743588
 * Nathaly Mesa García - 0750960
 */
#include "algoritmoGenetico.h"

//Constructor, recibe el grafo, la cantidad de ciudades, el tamaño de la población, el cosot máximo, el total de todas las aristas y el limite de generaciones.
algoritmoGenetico::algoritmoGenetico(int** grafoIN, int cantidadCiudadesIN, int tamanoPoblacionIN, int costoMaximoIN, int totalAristasIN, int cantGeneracionesIN)
{
	cantidadCiudades=cantidadCiudadesIN;
	costoMaximo=costoMaximoIN;
	tamanoPoblacion=tamanoPoblacionIN;
	totalAristas=totalAristasIN;
	cantGeneraciones=cantGeneracionesIN;
	
	grafo=new int*[cantidadCiudades];
	for(int i=0; i<cantidadCiudades; i++)
		grafo[i]=new int[cantidadCiudades];
	
	for(int i=0; i<cantidadCiudades; i++)
		for(int j=0; j<cantidadCiudades; j++)
			grafo[i][j]=grafoIN[i][j];
	
	optimo=0;
	generador=random_generator();
}

algoritmoGenetico::~algoritmoGenetico()
{
	for(int i = 0; i < cantidadCiudades; i++)
	{
		delete[] grafo[i];
		grafo[i]=0;
	}
	delete[] grafo;
	grafo=0;
	
	while(!poblacion.empty())
	{
		delete poblacion.back();
		poblacion.pop_back();
	}
	poblacion.clear();
	
	delete optimo;
	optimo=0;
	generacion=0;
}

//Función principal, produce nuevas generaciones hasta que hayOptimo() retorne true o se llegue al límite de generaciones.
void algoritmoGenetico::calcularSolucion()
{
	generacion=0;
	poblacionInicial();
	calcularAptitudPoblacion();
	
	while (!hayOptimo() && generacion<cantGeneraciones) //cambiar esto para que sea entrada también
	{
		nuevaGeneracion();
		calcularAptitudPoblacion();
		generacion++;
	}
}

//Genera la población inicial
void algoritmoGenetico::poblacionInicial()
{
	double valor; //valor a asignar al gen del cromosoma
	std::vector<double> arreglo; //arreglo del cromosoma a crear
	
	for(int i=0; i<tamanoPoblacion; i++)
	{
		for(int j=0; j<cantidadCiudades; j++)
		{
			valor=valor=generador.generarNumero();
			arreglo.push_back(valor);
			
		}
		poblacion.push_back(new cromosoma(arreglo));
		arreglo.clear();
	}
}

//Asigna a cada cromosoma el valor de su aptitud.
void algoritmoGenetico::calcularAptitudPoblacion()
{
	for(int i=0; i<tamanoPoblacion; i++)
	{
		//Solamente calcula la aptitud de los nuevos cromosomas en la población
		if(poblacion[i]->getAptitud() == 0)
			poblacion[i]->setAptitud(calcularAptitudCromosoma(i));
	}
}

//Usado por calcularAptitudPoblacion(). Devuelve el valor de la aptitud del cromosoma que está en la posición idx de la población.	
int algoritmoGenetico::calcularAptitudCromosoma(int idx)
{
	//Aptitud=pesoAcumulado+penalizacion
	int pesoAcumulado=0;//Costo de visitar las ciudades, siempre y cuando una ciudad sean adyacente a la que la precede en el orden indicado.
	int penalizacion=0; //ciudades no visitadas (a partir de la primera no adyacente a la ultima ciudad alcanzada) * el peso total de las aristas.
	int ciudadesNoVisitadas=cantidadCiudades;
	
	//indica el orden en el que se visitan las ciudades de acuerdo al cromosoma
	std::vector< std::pair<int,double> > camino;
	
	for(int i=0; i<cantidadCiudades; i++)
	{
		camino.push_back(std::pair<int,double>(i,poblacion[idx]->getGen(i)));
	}
	
	std::sort(camino.begin(), camino.end(), miOrden);
	
	//Calcular el peso acumulado
	int costoArista=0;
	for(int i=0; i<cantidadCiudades; i++)
	{
		if(i==cantidadCiudades-1)costoArista=grafo[camino[i].first][camino[0].first];
		else costoArista=grafo[camino[i].first][camino[i+1].first];
		
		if(costoArista != -1)
		{
			pesoAcumulado+=costoArista;
			ciudadesNoVisitadas--;
		}
		else
			break;
	}
	
	penalizacion=ciudadesNoVisitadas*totalAristas;
	return pesoAcumulado+penalizacion;
}

//Determina si no se puede mejorar la solución encontrada.
bool algoritmoGenetico::hayOptimo()
{
	//Sacar el mejor individuo de la poblacion
	if(optimo!=0) delete optimo;
	optimo=new cromosoma(poblacion[0]);
	for(int i=1; i<tamanoPoblacion; i++)
	{
		//Si el costo de la mejro sol. actual es mayor que la del cromosoma i
		if(poblacion[i]->getAptitud() < optimo->getAptitud())
		{
			delete optimo;
			optimo=0;
			optimo=new cromosoma(poblacion[i]);
		}
	}
	
	
	if(costoMaximo!=-1) //Si hay que encontrar un costo menor al valor de entrada
	{
		if(optimo->getAptitud() <= costoMaximo)
			return true;
		
		else
			return false;
	}
	else //Si hay que encontrar el mínimo posible
	{
		//ultimosMejores fnciona como una cola de tamaño n
		if(ultimosMejores.size()==tamanoPoblacion)
		{
			ultimosMejores.erase(ultimosMejores.begin());
			ultimosMejores.push_back(optimo->getAptitud());
			
			//Si las ultimas n generaciones han tenido la misma mejor solución -> true, else false; donde n= tamaño de la población
			for (int it=0; it<ultimosMejores.size()-1; it++)
			{
				if(ultimosMejores[it]!=ultimosMejores[it+1])
					return false;
			}
			return true;
		}
		else
		{
			ultimosMejores.push_back(optimo->getAptitud());
		}
		return false;
	}
}

//Produce una nueva generación a partir de la anterior.
void algoritmoGenetico::nuevaGeneracion()
{
	seleccionarCromosomas();
	cruzarSeleccion();
	mutarSeleccion();
	
	//Eliminar los peores cromosomas
	while(!poblacion.empty())
	{
		delete poblacion.back();
		poblacion.pop_back();
	}
	
	//Reemplazarlos por los nuevos
	while(!mattingPool.empty())
	{
		poblacion.push_back(mattingPool.back());
		mattingPool.pop_back();
	}
}

//Usado por nuevaGeneracion(). Selecciona el 60% de la población anterior por medio de torneo.
void algoritmoGenetico::seleccionarCromosomas()
{
	int tamanoSeleccion=(int)ceil(0.6*tamanoPoblacion);
	int cromosoma1, cromosoma2;
	
	for(int i=0; i<tamanoSeleccion; i++)
	{
		cromosoma1=(int)floor(generador.generarNumero()*(tamanoPoblacion-i));//posición del primer contrincante
		cromosoma2=(int)floor(generador.generarNumero()*(tamanoPoblacion-i));//posición del segundo contrincante
		
		//Pasar al matting pool el mejor de los dos
		if(poblacion[cromosoma1]->getAptitud() < poblacion[cromosoma2]->getAptitud())
		{
			mattingPool.push_back(poblacion[cromosoma1]);
			poblacion.erase(poblacion.begin()+cromosoma1);
		}
		else
		{
			mattingPool.push_back(poblacion[cromosoma2]);
			poblacion.erase(poblacion.begin()+cromosoma2);
		}
	}
}

//Usado por nuevaGeneracion(). Cruza los cromosomas seleccionados.
void algoritmoGenetico::cruzarSeleccion()
{
	int padre, madre;
	int tamanoMattingPool=mattingPool.size();
	
	for(int i=tamanoMattingPool; i<tamanoPoblacion; i++)
	{
		//Indices del padre y la madre del cromosoma a generar.
		padre=(int)floor(generador.generarNumero()*tamanoMattingPool);
		madre=(int)floor(generador.generarNumero()*tamanoMattingPool);
		generarHijo(padre, madre);
	}
}

//Usado por cruzarSeleccion(). Produce un individuo a partir de otros dos por medio de cruce uniforme.
void algoritmoGenetico::generarHijo(int padre, int madre)
{
	std::vector<double> arregloHijo;
	int gen;
	
	for(int i=0; i<cantidadCiudades; i++)
	{
		gen=(int)floor(generador.generarNumero()*2);
		if(gen==1)//Gen del padre
			arregloHijo.push_back(mattingPool[padre]->getGen(i));
		else//Gen de la madre
			arregloHijo.push_back(mattingPool[madre]->getGen(i));
	}
	
	mattingPool.push_back(new cromosoma(arregloHijo));
}

//Usado por nuevaGeneracion(). Muta el 1% de la población con respecto a la cantidad de ciudades.
void algoritmoGenetico::mutarSeleccion()
{
	int cantidadCromosmasMutar=ceil(0.01*cantidadCiudades*tamanoPoblacion);
	int genMutar; //indice del gen a mutar
	int cromosomaMutar;//indice del cromosoma a mutar
	
	for(int i=0; i<cantidadCromosmasMutar; i++)
	{
		cromosomaMutar=(int)floor(generador.generarNumero()*tamanoPoblacion);
		genMutar=(int)floor(generador.generarNumero()*cantidadCiudades);
		poblacion[cromosomaMutar]->mutarGen(genMutar);
	}
}

//Retorna un objeto solucion con el camino, el costo y la generación en la que se encontro la mejor solución.
solucion algoritmoGenetico::getSolucion()
{
	//Entregar las ciudades del cromosoma de la mejor solución en el orden en el que se visitan
	std::vector< std::pair<int,double> > camino;
	for(int i=0; i<cantidadCiudades; i++)
	{
		camino.push_back(std::pair<int,double>(i,optimo->getGen(i)));
	}
	std::sort(camino.begin(), camino.end(), miOrden);
	
	//Demás datos de la solución
	solucion resp;
	resp.aptituSol=optimo->getAptitud();
	resp.generacionSol=generacion;
	resp.rutaSol=camino;
	return resp;
}