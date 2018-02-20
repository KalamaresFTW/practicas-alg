#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
//CONSTANTES
#define K 500

//Funciones auxiliares
void inicializar_semilla();
void aleatorio(int v[], int n);
void listar_vector(int v[], int n);
double microsegundos();
bool ordenado(int v[], int n);
//Funciones para inicializar vectores
void orden_ascendente(int v[], int n);
void orden_descendente(int v[], int n);
void orden_aleatorio(int v[], int n);
//Funciones de test
void test_aux(void (*funcion_ordenacion)(int [], int), int v[], int n);
void test_seleccion();
void test_shell();
//Funciones de ordenación
void ordenacion_seleccion(int v[], int n);
void ordenacion_shell(int v[], int n);

void test_tiempos_seleccion(int n, int veces, void (*ord)(int [], int));

void test_tiempo_shell_medio(int n, int veces, void (*ord)(int [], int));
void test_tiempo_shell(int n, int veces, void (*ord)(int [], int));

/* Se establece la semilla de una nueva serie de enteros
 * pseudo aleatorios */
void inicializar_semilla() {
	srand(time(NULL));
}

/* Se generan números pseudoaleatorios entre -n y n */
void aleatorio(int v[], int n) {
	int i, m=2*n+1;
	for (i=0; i < n; i++)
		v[i] = (rand() % m) - n;
}

void listar_vector(int v[], int n){
	printf("[");
	for (int i = 0; i < n; i++){
		if (i == n-1){
			printf("%d]\n",v[i]);
		} else {
			printf("%d, ",v[i]);
		}
    }
}

/* obtiene la hora del sistema en microsegundos */
double microsegundos() {
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0) return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}

bool ordenado(int v[], int n){
    int i;
    for (i = 0; i < n - 1; i++) {
        if (v[i] > v[i+1]) {
            return false;
        }
    }
    return true;
}

void orden_ascendente(int v[], int n){
	int i;
	for (i = 0; i < n; i++){
		v[i] = i;
	}
}
void orden_descendente(int v[], int n){
	int i,j;
	for (i=0,j=n-1;i<n;i++,j--) {
		v[i] = j;
	}
}
void orden_aleatorio(int v[], int n){
	inicializar_semilla();
	aleatorio(v, n);
}

void test_aux(void (*funcion_ord)(int [], int), int v[], int n){
    listar_vector(v, n);
    if (ordenado(v, n)) {
        printf("%s\n", "¿Ordenado? Si");
    } else {
        printf("%s\n", "¿Ordenado? No");
        funcion_ord(v, n);
        listar_vector(v, n);
        if (ordenado(v, n)) {
            printf("%s\n", "¿Ordenado? Si");
        } else {
            printf("%s\n", "¿Ordenado? No");
        }
    }
	printf("%s", "\n");
}

void ordenacion_seleccion(int v[], int n){
    int i, j, minj, minx;
    for (i = 0; i < n-1; i++) {
        minj = i;
        minx = v[i];
        for (j = i+1; j < n; j++) {
            if (v[j] < minx) {
                minj = j;
                minx = v[j];
            }
        }
        v[minj] = v[i];
        v[i] = minx;
    }
}

void test_seleccion(){
    int n = 20;
    int v[n];
    printf("%s\n", "Ordenación por seleccion con inicialización aleatoria");
	orden_aleatorio(v,n);
    test_aux(ordenacion_seleccion,v,n);

    printf("%s\n", "Ordenación por seleccion con inicialización descendente");
	orden_descendente(v,n);
    test_aux(ordenacion_seleccion, v, n);
}

void ordenacion_shell(int v[], int n){
    bool seguir = true; //por defecto true
    int i, j, tmp, incremento;
    incremento = n;
    do {
        incremento /= 2;
        for (i = incremento; i < n; i++) {
            tmp = v[i];
            j = i;
            seguir = true;
            while ((j-incremento >= 0) && (seguir)) {
                if (tmp < v[j-incremento]) {
                    v[j] = v[j-incremento];
                    j = j-incremento;
                } else {
                    seguir = false;
                }
            }
            v[j] = tmp;
        }
    } while(incremento != 1);
}

void test_shell(){
    int n = 20;
    int v[n];
    printf("%s\n", "Ordenación por shell con inicialización aleatoria");
	orden_aleatorio(v,n);
    test_aux(ordenacion_shell, v, n);
    printf("%s\n", "Ordenación por shell con inicialización descendente");
	orden_descendente(v,n);
    test_aux(ordenacion_shell, v, n);
}

void test_tiempos_seleccion(int n, int veces, void (*ord)(int [], int)){
	printf("%6s %4s %10s %4s %10s %4s %10s %4s %10s\n",
		"n", "","t(n)","","t(n)/n^1.8", "","t(n)/n^2", "", "t(n)/n^2.2");
	for (int i = 0; i < veces; i++){
		int vector[n];
		double c_inf, c_aju, c_sob, ta, tb, t, t1, t2, inf=1.8, aju=2, sup=2.2;
		ord(vector, n);
		ta = microsegundos();
		ordenacion_seleccion(vector, n);
		tb = microsegundos();
		t = tb - ta;
		if (t < K){
			ta = microsegundos();
			for (int i = 0; i < K; i++){
				ord(vector, n);
				ordenacion_seleccion(vector, n);
			}
			tb = microsegundos();
			t1 = tb-ta;
			ta = microsegundos();
			for (int i = 0; i < K; i++) ord(vector, n);
			tb = microsegundos();
			t2 = tb-ta;
			t = (t1 - t2)/K;
			c_inf = (t / pow(n,inf));
			c_aju = (t / pow(n,aju));
			c_sob = (t / pow(n,sup));
			printf("%6d %4s %10.5lf* %4s %3.7lf %4s %3.7lf %4s %3.7lf \n",
				n, "", t, "", c_inf, "", c_aju, "", c_sob);
		} else {
			c_inf = (t / pow(n,inf));
			c_aju = (t / pow(n,aju));
			c_sob = (t / pow(n,sup));
			printf("%6d %4s %10.5lf %4s %3.7lf %4s %3.7lf %4s %3.7lf \n",
				n, "", t, "", c_inf, "", c_aju, "", c_sob);
		}
		n*=2;
	}
}

void test_tiempo_shell_medio(int n, int veces, void (*ord)(int [], int)){
	printf("%6s %4s %10s %4s %10s %4s %10s %4s %10s\n",
		"n", "","t(n)","","t(n)/n", "","t(n)/n^1.25", "", "t(n)/n^1.6");
	for (int i = 0; i < veces; i++){
		int vector[n];
		double c_inf, c_aju, c_sob, ta, tb, t, t1, t2, inf=1, aju=1.25, sup=1.6;
		ord(vector, n);
		ta = microsegundos();
		ordenacion_shell(vector, n);
		tb = microsegundos();
		t = tb - ta;
		if (t < K){
			ta = microsegundos();
			for (int i = 0; i < K; i++){
				ord(vector, n);
				ordenacion_shell(vector, n);
			}
			tb = microsegundos();
			t1 = tb-ta;
			ta = microsegundos();
			for (int i = 0; i < K; i++) ord(vector, n);
			tb = microsegundos();
			t2 = tb-ta;
			t = (t1 - t2)/K;
			c_inf = (t / pow(n,inf));
			c_aju = (t / pow(n,aju));
			c_sob = (t / pow(n,sup));
			printf("%6d %4s %10.5lf* %4s %3.7lf %4s %3.7lf %4s %3.7lf \n",
				n, "", t, "", c_inf, "", c_aju, "", c_sob);
		} else {
			c_inf = (t / pow(n,inf));
			c_aju = (t / pow(n,aju));
			c_sob = (t / pow(n,sup));
			printf("%6d %4s %10.5lf %4s %3.7lf %4s %3.7lf %4s %3.7lf \n",
				n, "", t, "", c_inf, "", c_aju, "", c_sob);
		}
		n*=2;
	}
}

void test_tiempo_shell(int n, int veces, void (*ord)(int [], int)){
	printf("%6s %4s %10s %4s %10s %4s %10s %4s %10s\n",
		"n", "","t(n)","","t(n)/n", "","t(n)/n^1.15", "", "t(n)/n^1.5");
	for (int i = 0; i < veces; i++){
		int vector[n];
		double c_inf, c_aju, c_sob, ta, tb, t, t1, t2, inf=1, aju=1.15, sup=1.5;
		ord(vector, n);
		ta = microsegundos();
		ordenacion_shell(vector, n);
		tb = microsegundos();
		t = tb - ta;
		if (t < K){
			ta = microsegundos();
			for (int i = 0; i < K; i++){
				ord(vector, n);
				ordenacion_shell(vector, n);
			}
			tb = microsegundos();
			t1 = tb-ta;
			ta = microsegundos();
			for (int i = 0; i < K; i++) ord(vector, n);
			tb = microsegundos();
			t2 = tb-ta;
			t = (t1 - t2)/K;
			c_inf = (t / pow(n,inf));
			c_aju = (t / pow(n,aju));
			c_sob = (t / pow(n,sup));
			printf("%6d %4s %10.5lf* %4s %3.7lf %4s %3.7lf %4s %3.7lf \n",
				n, "", t, "", c_inf, "", c_aju, "", c_sob);
		} else {
			c_inf = (t / pow(n,inf));
			c_aju = (t / pow(n,aju));
			c_sob = (t / pow(n,sup));
			printf("%6d %4s %10.5lf %4s %3.7lf %4s %3.7lf %4s %3.7lf \n",
				n, "", t, "", c_inf, "", c_aju, "", c_sob);
		}
		n*=2;
	}
}
int main() {
	printf("Ordenación por selección\n");
	printf("%s\n", "Mejor caso (Lista ordenada ascendentemente)");
	test_tiempos_seleccion(500,9,orden_ascendente);
	printf("%s\n", "Caso medio (Lista con valores aleatorios)");
	test_tiempos_seleccion(500,9,orden_aleatorio);
	printf("%s\n", "Peor caso (Lista ordenada descendentemente)");
	test_tiempos_seleccion(500,9,orden_descendente);

	printf("Ordenación por shell\n");
	printf("%s\n", "Mejor caso (Lista ordenada ascendentemente)");
	test_tiempo_shell(500,9,orden_ascendente);
	printf("%s\n", "Caso medio (Lista con valores aleatorios)");
	test_tiempo_shell_medio(500,9,orden_aleatorio);
	printf("%s\n", "Peor caso (Lista ordenada descendentemente)");
	test_tiempo_shell(500,9,orden_descendente);

	/*
    //test_seleccion();
    //test_shell();
	int n = 20;
	int v[n];
	orden_descendente(v,n);
	listar_vector(v,n);*/

	return 0;
}
