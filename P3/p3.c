

//hacer free del malloc
//
//typedef in;
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
//CONSTANTES
#define K 500
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define NUM_ENTRADAS 19062
#define LONGITUD_DICCIONARIO 38197
#define NUM_EJECUCIONES 10

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

typedef struct entrada_ {
  int ocupada; //0 - vacia, 1 - ocupada, 2 - eliminada
  char clave [LONGITUD_CLAVE];
  char sinonimos [LONGITUD_SINONIMOS];
} entrada;

typedef struct {
  char clave [LONGITUD_CLAVE];
  char sinonimos [LONGITUD_SINONIMOS];
} item;

typedef int colisiones;
typedef int pos;
typedef entrada *tabla_cerrada;

//Funciones auxiliares
void inicializar_semilla();
double microsegundos();

//Funciones de dispersion
int dispersionA(char *clave, int tamTabla);
int dispersionB(char *clave, int tamTabla);

int leer_sinonimos(item datos[]);
int ndispersion(char *clave, int tamTabla);
void inicializar_cerrada(tabla_cerrada diccionario, int TAM);
//void obtenerClaveAleatoria(item datos[], int tam, char **clave_aleatoria);

colisiones insertar_cerrada(item elemento,
    tabla_cerrada diccionario,
    int TAM,
    int (*dispersion)(char *, int),
    long (*resolucion_colisiones)(int pos_inicial, int num_intento));

pos buscar_cerrada(char *clave,
    tabla_cerrada diccionario,
    int TAM,
    int (*dispersion)(char *, int),
    long (*resolucion_colisiones)(int pos_inicial, int num_intento),
    int *colisiones);

void mostrar_cerrada(tabla_cerrada diccionario, int TAM);

//Funciones Complejidad
//void test_tiempos_dispersionA(int n, int veces, void  (*dispersion)(char *, int));
//void test_tiempos_dispersionA(int n, int veces,  (*dispersion)(char *, int));

/*****************************Implementacion***********************************/

/* Se establece la semilla de una nueva serie de enteros
 * pseudo aleatorios */
void inicializar_semilla() {
	srand(time(NULL));
}

/* obtiene la hora del sistema en microsegundos */
double microsegundos() {
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0) return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}

int dispersionA(char *clave, int tamTabla) {
  int i, valor = clave[0], n = strlen(clave);
  for (i = 1; i < n; i++)
    valor += clave[i];
  return valor % tamTabla;
}

int dispersionB(char *clave, int tamTabla) {
  int i, n = MIN(8, strlen(clave));
  unsigned int valor = clave[0];
  for (i = 1; i < n; i++)
    valor = (valor<<5) + clave[i]; /* el desplazamiento de 5 bits equivale a */
  return valor % tamTabla; /* multipicar por 32 */
}

int leer_sinonimos(item datos[]) {
  char c;
  int i, j;
  FILE *archivo;
  if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
    printf("Error al abrir 'sinonimos.txt'\n");
    return(EXIT_FAILURE);
  }
  for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
    if ((c = fgetc(archivo)) != '\t') {
      printf("Error al leer el tabulador\n");
      return(EXIT_FAILURE);
    }
    for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
      if (j < LONGITUD_SINONIMOS - 1)
        datos[i].sinonimos[j] = c;
    }

    datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
  }
  if (fclose(archivo) != 0) {
    printf("Error al cerrar el fichero\n");
    return(EXIT_FAILURE);
  }
  return(i);
}

int ndispersion(char *clave, int tamTabla) {
  if (strcmp(clave, "ANA")==0) return 7;
  if (strcmp(clave, "JOSE")==0) return 7;
  if (strcmp(clave, "OLGA")==0) return 7;
  return 6;
}

void inicializar_cerrada(tabla_cerrada diccionario, int TAM){
  pos i;
  for (i=0; i < TAM; i++) {
    diccionario[i].ocupada=0;
    strcpy(diccionario[i].clave, "");
    strcpy(diccionario[i].sinonimos, "");
  }
}

long resolucion_lineal(int pos_inicial, int num_intento){
  return num_intento;
}

long resolucion_cuadratica(int pos_inicial, int num_intento){
  return pow(num_intento,2);
}

pos buscar_cerrada(char *clave,
    tabla_cerrada diccionario,
    int TAM,
    int (*dispersion)(char *, int),
    long (*resolucion_colisiones)(int pos_inicial, int num_intento),
    int *colisiones){

    pos posActual;
    long x;
    int i = 0;
    x=dispersion(clave,TAM);
    posActual=x;
    while ((diccionario[posActual].ocupada!=0)
        && (strcmp((diccionario[posActual].clave),clave))) {
        i++;
        posActual = (x + resolucion_colisiones(x,i)) % TAM;
    }
    if (colisiones != NULL) {
        (*colisiones) = i;
    }
    return posActual;
}

colisiones insertar_cerrada(item elemento,
    tabla_cerrada diccionario,
    int TAM,
    int (*dispersion)(char *, int),
    long (*resolucion_colisiones)(int pos_inicial, int num_intento)){
    int colisiones = 0;
    pos posInsert;

    posInsert=buscar_cerrada(elemento.clave, diccionario, TAM, dispersion,
        resolucion_colisiones, &colisiones);
    if (!diccionario[posInsert].ocupada) {
        strcpy(diccionario[posInsert].clave, elemento.clave);
        strcpy(diccionario[posInsert].sinonimos, elemento.sinonimos);
        diccionario[posInsert].ocupada = 1;
    }
    return colisiones;
}

void mostrar_cerrada(tabla_cerrada diccionario, int TAM){
    int i;
    printf("{\n");
    for (i = 0; i < TAM; i++) {
        if (diccionario[i].clave != NULL) {
            printf("%d - (%s)\n", i, diccionario[i].clave);
        } else {
            printf("%d - \n", i);
        }
    }
    printf("}\n");
}

void testCerradaLineal() {
	tabla_cerrada tc_lineal = (tabla_cerrada) malloc (11 * sizeof(entrada));
	int i, posicion, tam = 11, colisiones = 0;
	item elemento;
	char claves[][7] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
	inicializar_cerrada(tc_lineal, tam);
    printf("***TABLA CERRADA LINEAL\n");
    for (i = 0; i < 6; i++){
		strcpy(elemento.clave, claves[i]);
		strcpy(elemento.sinonimos, "");
        colisiones += insertar_cerrada(elemento, tc_lineal, tam, ndispersion,
            resolucion_lineal);
	}
    mostrar_cerrada(tc_lineal,tam);
    printf("Colisiones: %d\n", colisiones);
    for(i = 0; i < 7; i++){
        posicion = buscar_cerrada(claves[i], tc_lineal, tam, ndispersion,
            resolucion_lineal, NULL);
        if (!strcmp(tc_lineal[posicion].clave, "")) {
            printf("No encuentro %s\n", claves[i]);
        } else {
            printf("Al buscar %s encuentro %s.\n", claves[i],
                tc_lineal[posicion].clave);
        }
    }
    free(tc_lineal);
}

void testCerradaCuadratica() {
    tabla_cerrada tc_cuadratica = (tabla_cerrada) malloc (11 * sizeof(entrada));
	int i, posicion, tam = 11, colisiones = 0;
	item elemento;
	char claves[][7] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    inicializar_cerrada(tc_cuadratica, tam);
    printf("\n***TABLA CERRADA CUADRATICA\n");
    for (i = 0; i < 6; i++){
        strcpy(elemento.clave, claves[i]);
        strcpy(elemento.sinonimos, "");
        colisiones += insertar_cerrada(elemento, tc_cuadratica, tam, ndispersion,
            resolucion_cuadratica);
    }
    mostrar_cerrada(tc_cuadratica, tam);
    printf("Colisiones: %d\n", colisiones);
    for(i = 0; i < 7; i++){
        posicion = buscar_cerrada(claves[i], tc_cuadratica, tam, ndispersion,
            resolucion_cuadratica, NULL);
        if (!strcmp(tc_cuadratica[posicion].clave, "")) {
            printf("No encuentro %s\n", claves[i]);
        } else {
            printf("Al buscar %s encuentro %s.\n", claves[i],
                tc_cuadratica[posicion].clave);
        }
    }
    free(tc_cuadratica);
}

int colisionesTotales(item datos[],
    int tam,
    tabla_cerrada diccionario,
    int (*dispersion)(char *, int),
    long (*resolucion_colisiones)(int pos_inicial, int num_intento)){
    int i = 0, colisiones = 0;
    item elemento;
    for (i = 0; i < tam; i++) {
        strcpy(elemento.clave, datos[i].clave);
        strcpy(elemento.sinonimos, datos[i].sinonimos);
        colisiones += insertar_cerrada(elemento, diccionario,
            LONGITUD_DICCIONARIO, dispersion, resolucion_colisiones);
    }
    return colisiones;
}

void testTotalColisiones(){
    tabla_cerrada d;
    int colisiones;
    item datos[NUM_ENTRADAS];
    printf("\n***TEST TOTAL COLISIONES\n");
    d = malloc (LONGITUD_DICCIONARIO * sizeof(entrada));
    leer_sinonimos(datos);
    inicializar_cerrada(d, LONGITUD_DICCIONARIO);
    colisiones = colisionesTotales(datos, NUM_ENTRADAS, d, dispersionA,
        resolucion_lineal);
    printf("Colisiones totales con dispersionA y resolucion lineal: %d\n",
        colisiones);
    inicializar_cerrada(d, LONGITUD_DICCIONARIO);
    colisiones = colisionesTotales(datos, NUM_ENTRADAS, d, dispersionB,
        resolucion_lineal);
    printf("Colisiones totales con dispersionB y resolucion lineal: %d\n",
      colisiones);
    inicializar_cerrada(d, LONGITUD_DICCIONARIO);
    colisiones = colisionesTotales(datos, NUM_ENTRADAS, d, dispersionA,
        resolucion_cuadratica);
    printf("Colisiones totales con dispersionA y resolucion cuadratica: %d\n",
            colisiones);
    inicializar_cerrada(d, LONGITUD_DICCIONARIO);
    colisiones = colisionesTotales(datos, NUM_ENTRADAS, d, dispersionB,
        resolucion_cuadratica);
    printf("Colisiones totales con dispersionB y resolucion cuadratica: %d\n",
                    colisiones);
    free(d);
}

void testBusqueda(int n,
    tabla_cerrada tc,
    item datos[],
    int (*dispersion)(char *, int),
    long (*resolucion_colisiones)(int pos_inicial, int num_intento),
    double inf, double aju, double sup){
    int r, i, j;
    char *claves_aleatorias[n];
    double c_inf, c_aju, c_sob, ta, tb, t, t1, t2;
    for (i = 0; i < n; i++) { //Inicializamos el array con claves aleatorias.
        r = (rand() % NUM_ENTRADAS);
        claves_aleatorias[i] = datos[r].clave;
    }
    ta = microsegundos();
    for (i = 0; i < n; i++) {
        buscar_cerrada(claves_aleatorias[i], tc, LONGITUD_DICCIONARIO,
            dispersion, resolucion_colisiones, NULL);
    }
    tb = microsegundos();
    t = tb - ta;
    if (t < K ) {
        printf("*");
        ta = microsegundos();
        for (j = 0; j < K; j++){
            for (i = 0; i < n; i++) { //Aleatorizamos el array
                r = (rand() % NUM_ENTRADAS);
                claves_aleatorias[i] = datos[r].clave;
            }
            for (i = 0; i < n; i++) {
                buscar_cerrada(claves_aleatorias[i], tc, LONGITUD_DICCIONARIO,
                    dispersion, resolucion_colisiones, NULL);
            }
        }
        tb = microsegundos();
        t1 = tb-ta;
        ta = microsegundos();
        for (i = 0; i < n; i++) { //Aleatorizamos el array
            r = (rand() % NUM_ENTRADAS);
            claves_aleatorias[i] = datos[r].clave;
        }
        tb = microsegundos();
        t2 = tb-ta;
        t = (t1 - t2)/K;
    }
    c_inf = (t / pow(n,inf));
    c_aju = (t / pow(n,aju));
    c_sob = (t / pow(n,sup));
    printf("%6d %4s %10.5lf %4s %3.7lf %4s %3.7lf %4s %3.7lf \n",
        n, "", t, "", c_inf, "", c_aju, "", c_sob);
}

void testTiemposBusqueda(int (*dispersion)(char *, int),
    long (*resolucion_colisiones)(int pos_inicial, int num_intento),
    double inf, double aju, double sup){
    item datos[NUM_ENTRADAS];
    tabla_cerrada tc;
    int i, n = 125;
    tc = (tabla_cerrada) malloc (LONGITUD_DICCIONARIO * sizeof(entrada));
    inicializar_cerrada(tc, LONGITUD_DICCIONARIO);
    leer_sinonimos(datos);
    colisionesTotales(datos,NUM_ENTRADAS,tc,dispersion,
        resolucion_colisiones); // Usamos esta funcion para insertar todo
                                // el diccionario dentro de la tabla.
    for (i = 0; i < NUM_EJECUCIONES; i++) {
        testBusqueda(n, tc, datos, dispersion, resolucion_colisiones,
                inf, aju, sup);
        n *= 2;
    }
    free(tc);
}

int main() {
    inicializar_semilla();
    testCerradaLineal();
    testCerradaCuadratica();
    testTotalColisiones();
    printf("\n\n***TEST TIEMPOS BUSQUEDA: DISPERSION A, LINEAL\n");
    printf("%6s %12s %18s %12s %18s \n", "n", "t(n)", "t(n)/n^0.9",
        "t(n)/n^2.25", "t(n)/n^2.55" );
    testTiemposBusqueda(dispersionA, resolucion_lineal, 0.90f, 2.25f, 2.55f);
    printf("\n\n***TEST TIEMPOS BUSQUEDA: DISPERSION B, LINEAL\n");
    printf("%6s %12s %18s %12s %18s \n", "n", "t(n)", "t(n)/n^0.8",
        "t(n)/n^1.4", "t(n)/n^1.5" );
    testTiemposBusqueda(dispersionB, resolucion_lineal, 0.8f, 1.4f, 1.5f);
    printf("\n\n***TEST TIEMPOS BUSQUEDA: DISPERSION A, CUADRATICA\n");
    printf("%6s %12s %18s %12s %18s \n", "n", "t(n)", "t(n)/n^0.9",
        "t(n)/n^1.26", "t(n)/n^1.6");
    testTiemposBusqueda(dispersionA, resolucion_cuadratica, 0.9f, 1.26f, 1.6f);
    printf("\n\n***TEST TIEMPOS BUSQUEDA: DISPERSION B, CUADRATICA\n");
    printf("%6s %12s %18s %12s %18s \n", "n", "t(n)", "t(n)/n^0.8", "t(n)/n",
        "t(n)/n^1.16" );
    testTiemposBusqueda(dispersionB, resolucion_cuadratica, 0.8f, 1.0f, 1.16f);
    return 0;
}
