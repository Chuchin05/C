#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NumCasillas 8
static char *vacio= "          ";
static char *borrado="**********";
typedef char** diciconario;


//Se crea el diccioanrio
diciconario crear() {
    diciconario dict = (char**)malloc(sizeof(char*)*NumCasillas);
    for (int i = 0; i < NumCasillas; i++) {
        dict[i] = vacio;
    }
    return dict;
}
 //se calcula h dependiendo la palabra y devuleve la poscion en la tabla hash
int h(char* cadena){
    int i = 0;
    int sum= 0;
    int tot=0;
    while (cadena[i] != '\0')
    {
        sum= sum + (int)cadena[i]; //conveirte la letra en un valor ASCII
        i++;
    }
    tot=sum % NumCasillas;
    return tot;
}
// se fija si existe la palabra en el diccionario, si no la encuentra devuelve VACIO
int Localiza(diciconario dic, char* cadena){
    int i = 0; 
    int inicial= h(cadena);
    int tot;
    while (i < NumCasillas && strcmp(dic[(inicial + i) % NumCasillas], cadena) != 0 && strcmp(dic[(inicial + i) % NumCasillas], vacio) != 0) {
    //la primer parte verifica si la cadena en la posicion hash no es igaul a la cadena bsucada
    //La segunda parte verifica si la posicion actual no esta vacia
    i++;
    }
    tot = (inicial+i) % NumCasillas;
    return tot; // devuelve el indice dodne encontro la cadena o una posicion vacia
}

//se fija si hay lugar donde se pueda insertar la cadena 
int Localiza1(diciconario dic, char* cadena){
    int i = 0;
    int inicial = h(cadena);
    while (i < NumCasillas && strcmp(dic[(inicial + i) % NumCasillas], cadena) != 0 && strcmp(dic[(inicial + i) % NumCasillas], vacio) != 0 && strcmp(dic[(inicial + i) % NumCasillas], borrado) != 0) {
    //igaul q la anterior pero verifica si la posicion actual no contiene la cadena BORRADO
        i++;
    }
    return (inicial+ i) %NumCasillas;
}

//verifica si la cadena es miembro o no del diccionario
int miembro(diciconario dic, char* cadena){
    if (strcmp(dic[Localiza(dic,cadena)],cadena)==0)
    {
        return 1;
    }else{
        return 0;
    }
    
}
//se agrega la palabra ak diccionario
void inserta(diciconario dic, char* cadena){
    int contenedor = 0;
    if (miembro(dic,cadena))
    {
        printf("Ya se encuentra en la lista\n");
        return;
    }
    contenedor = Localiza1(dic,cadena);
    //se pregunta si esta vacio

    if (strcmp(dic[contenedor],vacio)==0 || strcmp(dic[contenedor],borrado)==0)
    {
        dic[contenedor]= strdup(cadena); // copia la cadena
    }else{
        printf("Lista llena\n");
    }
    
}
//'Suprime' la palabra
void suprime(diciconario dic, char* cadena){
    int contenedor = Localiza(dic,cadena);
    if (strcmp(dic[contenedor],cadena)==0)
    {
        dic[contenedor]= borrado;
    }
    
}
//limpia la memoria dinamica del diccionario
void DestruirTabla(diciconario dic){
    for (int i = 0; i < NumCasillas; i++)
    {
        if (dic[i] != vacio && dic[i]!= borrado)
        {
            free(dic[i]);
        }
    }
    free(dic);
}

int main(){
    diciconario dic = crear();


    char *cadena1 = (char*)malloc(sizeof(char) * 11); //reserva memoria
    printf("Ingrese la primer cadena- ");
    fgets(cadena1, 10, stdin);//se guarda la cadena en "cadena1"
    inserta(dic, cadena1);//se agreaga en el diccionario


    char * cadena2 =(char*)malloc(sizeof(char)* 11);
    printf("Ingrese la segunda cadena- ");
    fgets(cadena2, 10,stdin);
    inserta(dic,cadena2);

    char * cadena3 =(char*)malloc(sizeof(char)* 11);
    printf("Ingrese la tercera cadena- ");
    fgets(cadena3, 10,stdin);
    cadena3[strcspn(cadena3,"\n")]= '\0';    
    inserta(dic,cadena3);

    printf("Se 'borra' la segunda cadena--\n");
    suprime(dic,cadena2); //se cambia el contenido de la segunda cadena

    char * cadena4 =(char*)malloc(sizeof(char)* 11);
    printf("Ingrese la cuarta cadena- ");
    fgets(cadena4, 10,stdin);
    cadena4[strcspn(cadena4,"\n")]= '\0';
    inserta(dic,cadena4);

    printf("Buscar cadena!- ");
    char * cadena5 =(char*)malloc(sizeof(char)* 11);
    fgets(cadena5,10,stdin);

    if ((miembro(dic,cadena5)))
    {
        printf("Se encuentra en la lista %s",cadena5);
    }else{
        printf("No se encuentra en la lista %s",cadena5);
    }
    

    printf("Se imprmime la lista Hasheada\n");
    for (int i = 0; i < NumCasillas; i++)
    {
        printf("%d. %s\n", i, dic[i]);
    }

    //libera memoria dinamica
    DestruirTabla(dic);
    free(cadena1);
    free(cadena2);
    free(cadena3);
    free(cadena4);
    free(cadena5);

    
    return 0;
}