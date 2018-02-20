#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

int sumaSubMax1(int v[], int n);
int sumaSubMax2(int v[], int n);
void inicializar_semilla();
void aleatorio(int v[], int n);
void listar_vector(int v[], int n);
double microsegundos();
void test1();
void test2();
void test_suma1(int longitud, int veces);
void test_suma2(int longitud, int veces);


/*
 * Cota subestimada: n^1,8
 * Cota subestimada: n^2
 * Cota subestimada: n^2,2
 */
int sumaSubMax1(int v[], int n){
	int i, j;
	int estaSuma = 0, sumaMax = 0;
	for (i = 0; i < n; i++) {
		estaSuma = 0;
		for (j = i; j < n; j++) {
			estaSuma += v[j];
			if (estaSuma > sumaMax) {
				sumaMax = estaSuma;
			}
		}
	}
	return sumaMax;
}

/*
 * Cota subestimada: n^0,8
 * Cota subestimada: n
 * Cota subestimada: n·log(n)
 */
int sumaSubMax2(int v[], int n){
	int estaSuma = 0, sumaMax = 0;
	for (int i = 0; i < n; i++){
		estaSuma += v[i];
		if (estaSuma > sumaMax){
			sumaMax = estaSuma;
		} else if (estaSuma < 0) {
			estaSuma = 0;
		}
	}
	return sumaMax;
}

/* Se establece la semilla de 
 * una nueva serie de enteros
 * pseudo aleatorios */
void inicializar_semilla() {
	srand(time(NULL));
}

/* Se generan números
 * pseudoaleatorios 
 * entre -n y n */
void aleatorio(int v[], int n) {
	int i, m=2*n+1;
	for (i=0; i < n; i++)
		v[i] = (rand() % m) - n;
}


void listar_vector(int v[], int n){
	printf("[");
	for (int i = 0; i < n; i++){
		if (i == n-1){
			printf("%d]",v[i]);
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

void test1(){
	int i;
	int a, b;
	int v[][5] = {
		{-9, 2, -5, -4, 6},
		{4, 0, 9, 2, 5},
		{-2, -1, -9, -7, -1},
		{9, -2, 1, -7, -8},
		{15, -2, -5, -4, 16},
		{7, -5, 6, 7, -7}
	};
	printf("test 1\n");
	printf("%15s%20s%20s\n", "", "sumaSubMax1", "sumaSubMax2");
	for (i=0; i<6; i++) {
		listar_vector(v[i], 5);
		a = sumaSubMax1(v[i], 5);
		b = sumaSubMax2(v[i], 5);
		printf("%20d%10d\n", a, b);
	}
}

void test2(){
	inicializar_semilla();
	int i;
	long a,b;
	int v[5];
	printf("test\n");
	printf("%18s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
	for (i=0; i<5; i++){
		aleatorio(v,5);
		listar_vector(v,5);
		a = sumaSubMax1(v,5);
		b = sumaSubMax2(v,5);
		printf("%15ld%15ld\n",a,b);
	}
}


void test_suma1(int longitud, int veces){
	printf("sumaSubMax1\n");
	printf("%6s %4s %10s %4s %10s %4s %10s %4s %10s\n", "n", "","t(n)","","t(n)/n^1.8", "","t(n)/n^2", "", "t(n)/n^2.2");
	for (int i = 0; i < veces; i++){	
		int vector[longitud];
		aleatorio(vector, 100);
		double ta, tb, t;
		double c_inf, c_aju, c_sob;
		ta = microsegundos();
		sumaSubMax1(vector, longitud);
		tb = microsegundos();
		t = tb - ta;
		if (t < 500){
			double t1, t2;
			ta = microsegundos();
			for (int i = 0; i < 500; i++){
				aleatorio(vector, 100);
				sumaSubMax1(vector, longitud);
			}
			tb = microsegundos();
			t1 = tb-ta;
			ta = microsegundos();
			for (int i = 0; i < 500; i++){
				aleatorio(vector, 100);
			}
			tb = microsegundos();
			t2 = tb-ta;
			t = (t1 - t2)/500;
			
			c_inf = (t / pow(longitud,1.8));
			c_aju = (t / pow(longitud,2));
			c_sob = (t / pow(longitud,2.2));

			printf("%6d %4s %10.3lf* %4s %3.7lf %4s %3.7lf %4s %3.7lf \n", longitud, "", t, "", c_inf, "", c_aju, "", c_sob);
		} else {
			c_inf = (t / pow(longitud,1.8));
			c_aju = (t / pow(longitud,2));
			c_sob = (t / pow(longitud,2.2));

			printf("%6d %4s %10.3lf %4s %3.7lf %4s %3.7lf %4s %3.7lf \n", longitud, "", t, "", c_inf, "", c_aju, "", c_sob);
		}
		
		longitud *= 2;
	}
}

void test_suma2(int longitud, int veces){
	printf("sumaSubMax2\n");
	printf("%6s %4s %10s %4s %10s %4s %10s %4s %10s\n", "n", "","t(n)","","t(n)/n^1.8", "","t(n)/n", "", "t(n)/nlogn");
	for (int i = 0; i < veces; i++){	
		int vector[longitud];
		aleatorio(vector, 100);
		double ta, tb, t;
		double c_inf, c_aju, c_sob;
		ta = microsegundos();
		sumaSubMax2(vector, longitud);
		tb = microsegundos();
		t = tb - ta;
		if (t < 500){
			double t1, t2;
			ta = microsegundos();
			for (int i = 0; i < 500; i++){
				aleatorio(vector, 100);
				sumaSubMax2(vector, longitud);
			}
			tb = microsegundos();
			t1 = tb-ta;
			ta = microsegundos();
			for (int i = 0; i < 500; i++){
				aleatorio(vector, 100);
			}
			tb = microsegundos();
			t2 = tb-ta;
			t = (t1 - t2)/500;
			
			c_inf = (t / pow(longitud,0.8));
			c_aju = (t / longitud);
			c_sob = (t / (log(longitud)*longitud));

			printf("%6d %4s %10.5lf* %4s %3.7lf %4s %3.7lf %4s %3.7lf \n", longitud, "", t, "", c_inf, "", c_aju, "", c_sob);
		} else {
			c_inf = (t / pow(longitud,0.8));
			c_aju = (t / longitud);
			c_sob = (t / (log(longitud)*longitud));

			printf("%6d %4s %10.5lf %4s %3.7lf %4s %3.7lf %4s %3.7lf \n", longitud, "", t, "", c_inf, "", c_aju, "", c_sob);
		}
		
		longitud *= 2;
	}
}





int main(int argc, char **argv){
	int veces = 7;
	test_suma1(500,veces);
	test_suma1(500,veces);
	test_suma1(500,veces);
	test_suma1(500,veces);

	veces = 13;
	test_suma2(500,veces);
	test_suma2(500,veces);
	test_suma2(500,veces);
	test_suma2(500,veces);
	return 0;
}