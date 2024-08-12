#include<stdio.h>
#include<stdlib.h>

typedef struct arista{
    char u; //origen
    char v; //destino
    int peso;
    struct arista* siguiente;
}Arista;

typedef struct nodo{
char nodo;
struct Arista* adyacente;
struct nodo* siguiente;
}Nodo;

typedef struct grafo{
    Nodo* Tnodos;
    Arista* Taristas;
}Grafo;


//Conjuntos disjuntos
typedef struct conjunto{
    char padre;
    int nivel;
}Conjunto;

Nodo* CrearNodo(char nombre){
    Nodo* nuevonodo= (Nodo*)malloc(sizeof(Nodo));
    nuevonodo->adyacente=NULL;
    nuevonodo->siguiente=NULL;
    nuevonodo->nodo=nombre;
    return nuevonodo;
}
Arista* CrearArista(char origen, char destino, int peso){
    Arista* nuevaArista= (Arista*)malloc(sizeof(Arista));
    nuevaArista->u=origen; 
    nuevaArista->v=destino;
    nuevaArista->peso=peso;
    nuevaArista->siguiente=NULL;

    return nuevaArista;
}

void AgregarNodo(Grafo* arbol, char nodo){
    Nodo* nuevoNodo= CrearNodo(nodo);
    nuevoNodo->siguiente= arbol->Tnodos; //se agrega a todos los nodos del arbol
    arbol->Tnodos= nuevoNodo; // el nuevo nodo es el primero de todos los nodos
    printf("Se agrego el Nodo %c al arbol\n", nodo);
}

void AgregarArista(Grafo* arbol, char origen, char destino,int peso){
    Arista* nuevaArista = CrearArista(origen,destino,peso);
    nuevaArista->siguiente= arbol->Taristas; //se agrega la arista a todas las aristas
    arbol->Taristas= nuevaArista;//la nueva arista es la cabeza de todas las aristas
    printf("Se agrego la Arista que va desde %c < --%d-- > %c \n",origen,peso,destino);
}

//Se va a llamar recursivamente hasta q todos los nodos tengan de hijo a A
char Encontrar(Conjunto conjuntos[],char nodo){
    if (conjuntos[nodo-'A'].padre != nodo)
    {
        conjuntos[nodo - 'A'].padre = Encontrar(conjuntos, conjuntos[nodo - 'A'].padre);
    }
    return conjuntos[nodo - 'A'].padre;
}
/*
    conjuntos[0]={padre: 'A', altura : 0}
    conjuntos[1]={padre: 'B', altura : 0}
    conjuntos[2]={padre: 'C', altura : 0}
    //
        conjunto [ 'C' - 'A'] es conjunto[2] pq tiene de padre a C
        conjunto [ 'B' - 'A'] es conjunto[1] pq tiene de padre a B
        conjunto [ 'A' - 'A'] es conjunto[0] pq tiene de padre  A
    //
Quedaria asi dsp de la llama recursiva
conjuntos[0] = {padre: 'A', altura: 0}
conjuntos[1] = {pader: 'A', altura: 0}
conjuntos[2] = {padre: 'A', altrua: 0}
*/

//funcion Para unir ods conjuntos
void Combina(Conjunto conjuntos[], char u, char v){
    char raizU= Encontrar(conjuntos, u);
    char raizV= Encontrar(conjuntos, v);
    //compara las raices, sui son las mismmas no la uno pq formaria una ciclo
    if (conjuntos[raizU - 'A'].nivel < conjuntos[raizV - 'A'].nivel)
    {
        conjuntos[raizU - 'A'].padre= raizV;
        //raizU se convierte en el hijo de raiz V
    }else if (conjuntos[raizU - 'A'].nivel > conjuntos[raizV - 'A'].nivel)
    {
        conjuntos[raizV - 'A'].padre = raizU;
    }
     else{
        conjuntos[raizV - 'A'].padre = raizU; //Raiz V es el hijo de raiz U
        conjuntos[raizU - 'A'].nivel++; //Se incrementa raiz U

    }
    
}
Arista* OrdenaArista(Arista* arista){
    if (arista == NULL || arista->siguiente==NULL)
    {
        return arista;
    }

    Arista* medio= arista;
    Arista* fin= arista->siguiente;
    while (fin != NULL && fin->siguiente != NULL)
    {
        medio = medio->siguiente;
        fin = fin->siguiente->siguiente;
    }
    //El puntero medio se mueve un paso a la vez mientras que fin se mueve dos pasos. Cuando fin llega al final, medio está en el medio de la lista.
    Arista* mitad = medio->siguiente;
    medio->siguiente=NULL;

    Arista* izquierda = OrdenaArista(arista);
    Arista* derecha = OrdenaArista(mitad);
    
    Arista* resultado = NULL;// Puntero para la lista ordenada resultante

    //Inicializa la lista ordenada con el menor de los dos elementos inciales
    if (izquierda->peso <= derecha->peso)
    {
        resultado= izquierda;
        izquierda=izquierda->siguiente;
    }else{
        resultado = derecha;
        derecha= derecha->siguiente;
    }
    //se ordenan las aristaws
    Arista* actual = resultado;
    //combina las dos mitades ordenandolas en una sola lista
    while (izquierda != NULL && derecha != NULL)
    {
        if (izquierda->peso <= derecha->peso)
        {
            actual->siguiente=izquierda;
            izquierda= izquierda->siguiente;
        }else
        {
            actual->siguiente= derecha;
            derecha= derecha->siguiente;
        }
    actual= actual->siguiente;
    }
    

    //si quedan elementos enla mitad izquierda, los añade al fuinal de la lista ordenada
    if  (izquierda !=NULL){
        actual->siguiente= izquierda;
    }else{
    //si quedan elementos enla mitad dere, los añade al fuinal de la lista ordenada

        actual->siguiente= derecha;
    }

    return resultado;

}

void imprimeAristas(Arista* arista){
    int caminoCorto=0;
    Arista* actual = arista;
    while (actual != NULL)
    {
        printf("La ARISTA va desde %c ----%d---- %c\n",actual->u, actual->peso, actual->v);
        caminoCorto= caminoCorto + actual->peso;
        actual = actual->siguiente;
    }
    
    printf("El camino mas corto tiene un valor de %d \n",caminoCorto);
}

void Kruskal(Grafo* arbol){
    Arista* resultadoFinal = NULL; //Lista enlazada con las aristas de menor peso
    int aristaCont = 0; //contador de aristas

    Arista* aristas = OrdenaArista(arbol->Taristas); //obtiene la lista de aristas ordenadas por peso usando la función OrdenaArista

    int numVertices=0;

    Nodo* actual = arbol->Tnodos;
    while (actual != NULL)
    {
        numVertices++; //se cuenta la cantidad de nodos q tiene el grafo
        actual= actual->siguiente;
    }
    
    Conjunto* conjuntos = (Conjunto*)malloc(numVertices*sizeof(Conjunto));
    for (int V = 0; V < numVertices; ++V)
    {
        conjuntos[V].padre= 'A' + V; //Asigna el caracter que representa al nodo como su propio padre
        conjuntos[V].nivel=0; //se declara nivel 0 para manejar mejor la union de los conjuntos
    }
    while (aristas != NULL && aristaCont<numVertices-1)
    {
        Arista* actualArista = aristas; // se obitene la arisat con menor peso
        aristas = aristas->siguiente;
        //seleccoiona la arista de menor peso y "Encuentra" los nodos conectados
        char u= Encontrar(conjuntos, actualArista->u); 
        char v= Encontrar(conjuntos, actualArista->v);
    //si son distintos se agrega al abrol
        if (u != v)
        {
            Arista* nuevaArista = CrearArista(actualArista->u,actualArista->v,actualArista->peso);
            nuevaArista->siguiente= resultadoFinal;
            resultadoFinal= nuevaArista;
            aristaCont++;
            Combina(conjuntos, u ,v);
        }
        
    }
    
    printf("\n\t Algoritmo de KRUSKAL\n\n");
    imprimeAristas(resultadoFinal);

    free(conjuntos);
}

int main() {
    Grafo grafo;
    grafo.Tnodos = NULL;
    grafo.Taristas = NULL;

    // Agregar nodos al grafo
    AgregarNodo(&grafo, 'A');
    AgregarNodo(&grafo, 'B');
    AgregarNodo(&grafo, 'C');
    AgregarNodo(&grafo, 'D');
    AgregarNodo(&grafo, 'E');
    AgregarNodo(&grafo, 'F');


    // Agregar aristas al grafo
    AgregarArista(&grafo, 'A', 'B', 6);
    AgregarArista(&grafo, 'A', 'C', 1);
    AgregarArista(&grafo, 'A', 'D', 5);
    AgregarArista(&grafo, 'B', 'C', 5);
    AgregarArista(&grafo, 'B', 'E', 3);
    AgregarArista(&grafo, 'D', 'C', 5);
    AgregarArista(&grafo, 'D', 'F', 2);
    AgregarArista(&grafo, 'E', 'F', 6);
    AgregarArista(&grafo, 'C', 'F', 4);
    AgregarArista(&grafo, 'C', 'E', 6);
    

    Kruskal(&grafo);

    return 0;
}
