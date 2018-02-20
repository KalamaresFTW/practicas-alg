/*
  Autores:
 -Pablo Costa Oubiña - p.costa
 -Alejandra Facal Regueiro - a.fregueiro
 -Grupo: 3.2  (Jueves 8:30 - 10:30)
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
//CONSTANTES
#define K 100

#ifndef TAM_MAX
#define TAM_MAX 1600
#endif

#ifndef NUM_EJECUCIONES
#define NUM_EJECUCIONES 7
#endif

#ifndef INFINITO
#define INFINITO INT_MAX
#endif

typedef int ** matriz;
typedef struct {
    int x, y, peso;
} arista;
typedef arista tipo_elemento;
typedef struct {
    int cabeza, final, tamano;
    tipo_elemento vector[TAM_MAX];
} cola;

//Funciones auxiliares
void inicializar_semilla();
double microsegundos();

//Funciones cola
void crear_cola(cola *);
bool cola_vacia(cola *);
void incrementar(int *);
void insertar(tipo_elemento, cola *);
tipo_elemento quitar_primero(cola *);
tipo_elemento primero(cola);
void mostrar_cola(cola *);

//Funciones Matriz
matriz crear_matriz(int);
void inicializar_matriz(matriz, int);
void liberar_matriz(matriz , int );


//Funciones Prim y test
void prim(matriz, int, cola *);
void testMatrizPrim1();
void testMatrizPrim2();
void testMatrizPrim3();
void tiempos(int nodos, double inf, double aju, double sup);

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

void crear_cola(cola * c){
    c->tamano = 0;
    c->cabeza = 1;
    c->final = TAM_MAX;
}

bool cola_vacia(cola * c){
    return c->tamano == 0;
}

void incrementar(int * x){
    if ((*x) == TAM_MAX) (*x) = 1;
    else (*x)++;
}

void insertar(tipo_elemento elemento, cola * c){
    if (c->tamano == TAM_MAX)
        printf("%s\n", "Error: cola llena.");
    else {
        c->tamano++;
        incrementar(&c->final);
        c->vector[c->final] = elemento;
    }
}

tipo_elemento quitar_primero(cola * c){
    tipo_elemento elemento;
    if (cola_vacia(c)) {
        printf("%s\n", "Error: cola vacia.");
    } else {
        c->tamano--;
        elemento = c->vector[c->cabeza];
        incrementar(&c->cabeza);
        return elemento;
    }
    return elemento;
}


/**
* En esta funcion imprimimo la cola con las aristas y muestro
* tambien el peso mínimo total
*/

void mostrar_cola(cola * c){
    int i, pesoTotal=0;
    if (cola_vacia(c)){
        printf ("La cola esta vacia\n");
    } else {
        printf ("\n\n x - y  -  peso \n");
        for (i = c->cabeza; i <=c->tamano ; i++) {
  		printf ("{%d - %d} -> %d \n",
            c->vector[i].x,
            c->vector[i].y,
            c->vector[i].peso);
  		pesoTotal += c->vector[i].peso;
    }
    printf ("peso total: %d\n", pesoTotal);
  }
}

/* calcular el árbol de recubrimiento mínimo devolviendo
    las aristas del arbol en la cola ’aristas’ */
void prim(matriz m, int nodos, cola * aristas) {
    int min, i, j, z, k=0;
    arista a;
    int *masProximo = (int *) malloc(nodos*sizeof(int));
    int *distanciaMinima = (int *) malloc(nodos*sizeof(int));
    crear_cola(aristas);
    distanciaMinima[0] = -1;
    for(i = 1; i < nodos; i++) {
        masProximo[i] = 0;
        distanciaMinima[i] = m[i][0];
    }
    for (z = 0; z < nodos-1; z++) {
        min = INFINITO;
        for (j = 1; j < nodos; j++) {
          if ((0 <= distanciaMinima[j])
                && (distanciaMinima[j] < min)) {
            min = distanciaMinima[j];
            k = j;
          }
        }
        a.x = masProximo[k];  //Para guardar las aristas del grafo T
        a.y = k;
        a.peso = m[a.x][k];
        insertar(a,aristas);  // T := T U {(MasProximo[k],k)};
        distanciaMinima[k]=-1;
        for (j = 1; j < nodos; j++) {
            if (m[j][k] < distanciaMinima[j]) {
                distanciaMinima[j] = m[j][k];
                masProximo[j] = k;
            }
        }
    }
    free(masProximo);
    free(distanciaMinima);
}

matriz crear_matriz(int n) {
    int i;
    matriz aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
        return aux;
}

void inicializar_matriz(matriz m, int n) {
    /* Crea un grafo completo no dirigido con valores aleatorios entre 1 y n */
    int i, j;
    for (i=0; i<n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % n + 1;
        for (i=0; i<n; i++)
            for (j=0; j<=i; j++)
                if (i==j)
                    m[i][j] = 0;
                else
                    m[i][j] = m[j][i];
}

    void liberar_matriz(matriz m, int n) {
      int i;
      for (i=0; i<n; i++)
        free(m[i]);
      free(m);
    }

void mostrar_matriz(matriz m, int n){
    int i,j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf(" %d ", m[i][j]);
        }
        printf("\n");
    }
}

void testMatrizPrim1(){
    cola c;
    matriz G = crear_matriz(5);
    G[0][0] = 0; G[1][0] = 1; G[2][0] = 8; G[3][0] = 4; G[4][0] = 7;
    G[0][1] = 1; G[1][1] = 0; G[2][1] = 2; G[3][1] = 6; G[4][1] = 5;
    G[0][2] = 8; G[1][2] = 2; G[2][2] = 0; G[3][2] = 9; G[4][2] = 5;
    G[0][3] = 4; G[1][3] = 6; G[2][3] = 9; G[3][3] = 0; G[4][3] = 3;
    G[0][4] = 7; G[1][4] = 5; G[2][4] = 5; G[3][4] = 3; G[4][4] = 0;
    printf("%s\n", "| -------------------- Test 1 -------------------- |");
    mostrar_matriz(G, 5);
    prim(G, 5, &c);
    mostrar_cola(&c);
    liberar_matriz(G, 5);
    printf("%s\n", "| ------------------------------------------------ |\n\n");
}

void testMatrizPrim2(){
    cola c;
    matriz G = crear_matriz(4);
    G[0][0] = 0; G[1][0] = 1; G[2][0] = 4; G[3][0] = 7;
    G[0][1] = 1; G[1][1] = 0; G[2][1] = 2; G[3][1] = 8;
    G[0][2] = 4; G[1][2] = 2; G[2][2] = 0; G[3][2] = 3;
    G[0][3] = 7; G[1][3] = 8; G[2][3] = 3; G[3][3] = 0;
    printf("%s\n", "| -------------------- Test 2 -------------------- |");
    mostrar_matriz(G, 4);
    prim(G, 4, &c);
    mostrar_cola(&c);
    liberar_matriz(G, 4);
    printf("%s\n", "| ------------------------------------------------ |\n\n");
}

void testMatrizPrim3(){
    cola c;
    matriz G = crear_matriz(7);
    G[0][0] = 0; G[1][0] = 7; G[2][0] = 99; G[3][0] = 5; G[4][0] = 99;
        G[5][0] = 99; G[6][0] = 99;

    G[0][1] = 7; G[1][1] = 0; G[2][1] = 8; G[3][1] = 9; G[4][1] = 7;
        G[5][1] = 99; G[6][1] = 99;

    G[0][2] = 99; G[1][2] = 8; G[2][2] = 0; G[3][2] = 99; G[4][2] = 5;
        G[5][2] = 99; G[6][2] = 99;

    G[0][3] = 5; G[1][3] = 9; G[2][3] = 99; G[3][3] = 0; G[4][3] = 15;
        G[5][3] = 6; G[6][3] = 99;

    G[0][4] = 99; G[1][4] = 7; G[2][4] = 5; G[3][4] = 15; G[4][4] = 0;
        G[5][4] = 8; G[6][4] = 9;

    G[0][5] = 99; G[1][5] = 99; G[2][5] = 99; G[3][5] = 6; G[4][5] = 8;
        G[5][5] = 0; G[6][5] = 11;

    G[0][6] = 99; G[1][6] = 99; G[2][6] = 99; G[3][6] = 99; G[4][6] = 9;
        G[5][6] = 11; G[6][6] = 0;
    printf("%s\n", "| -------------------- Test 3 -------------------- |");
    mostrar_matriz(G, 7);
    prim(G, 7, &c);
    mostrar_cola(&c);
    liberar_matriz(G, 7);
    printf("%s\n", "| ------------------------------------------------ |\n\n");
}

void tiempos(int nodos, double inf, double aju, double sup){
    matriz m;
    cola c;
    int i, j;
    double c_inf, c_aju, c_sob, ta, tb, t, t1, t2;
    m = crear_matriz(nodos);
    inicializar_matriz(m,nodos);
    ta = microsegundos();
    prim(m, nodos, &c);
    tb = microsegundos();
    t = tb - ta;
    if (t < 500) {
        printf("*");
        ta = microsegundos();
        for (j = 0; j < K; j++){
            inicializar_matriz(m, nodos);
            prim(m, nodos, &c);
        }
        tb = microsegundos();
        t1 = tb-ta;
        ta = microsegundos();
        for (i = 0; i < K; i++)
            inicializar_matriz(m, nodos);
        tb = microsegundos();
        t2 = tb-ta;
        t = (t1 - t2)/K;
    }
    c_inf = (t / pow(nodos,inf));
    c_aju = (t / pow(nodos,aju));
    c_sob = (t / pow(nodos,sup));
    printf("%7s %6d %6s %10.5lf %6s %3.7lf %6s %3.7lf %6s %3.7lf\n",
        "", nodos, "", t, "", c_inf, "", c_aju, "", c_sob);
    liberar_matriz(m,nodos);
}

void testTiempos(){
    int nodos = 10, i;
    printf("%12s", "------------------------------------------------------\n");
    printf("%15s %17s %16s %16s %17s",
        "n", "t(n)", "t(n)/n^1.7", "t(n)/n^2", "t(n)/n^2.2\n");
    for (i = 0; i < NUM_EJECUCIONES; i++) {
        tiempos(nodos,1.7f, 2.0f, 2.2f);
        nodos *= 2;
    }
}

int main() {
    inicializar_semilla();
    testMatrizPrim1();
    testMatrizPrim2();
    testMatrizPrim3();
    testTiempos();
    testTiempos();
    testTiempos();
    testTiempos();
}
