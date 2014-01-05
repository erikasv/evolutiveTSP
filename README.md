EvolutiveTSP
============

An approach by genetic algorithms to the Traveling Salesman Problem.
--------------------------------------------------------------------

Para ejecutar el programa, desde una consola:

1. Ingrese a la carpeta "codigoFuente" y ejecute el comando "make".
2. Ejecute "vendedorViajero &lt;nombre_archivo_grafo&gt; 
&lt;parametros_opcionales&gt;":

* &lt;nombre_archivo_grafo&gt; es el nombre del archivo (.txt) en donde está la información del grafo de la siguiente forma:
  * &lt;cantidadNodos&gt; &lt;cantidadAristas&gt; &lt;listaAristas&gt;, con &lt;listaAristas&gt; una lista de largo &lt;cantidadAristas&gt; donde un elemento es de la forma &lt;Nodo1&gt; &lt;Nodo2&gt; &lt;PesoArista&gt;
* &lt;parametros_opcionales&gt; es un subconjunto (puede ser vacío) de: 
  * &lt;-t tamañoPoblacion&gt; &lt;-c costoMaximo&gt; &lt;-g cantidadGeneraciones&gt;
   Por ejemplo: ./vendedorViajero ../ejemplos/ejemplo2.txt -c 20
