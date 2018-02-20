#Autores:
# -Pablo Costa Oubiña - p.costa
# -Alejandra Facal Regueiro - a.fregueiro
#Grupo: 3.2  (Jueves 8:30 - 10:30)
CFLAGS = -Wall -Wextra

all: clean
	gcc $(CFLAGS) -o p2 p2.c -lm

#Elimina todos los binarios(sin extensión), *.o y *.tar.gz.
clean:
	ls | grep -v "\." | grep -v Makefile | xargs rm -f *.o $(TARNAME).tar.gz

valgr:
	valgrind --leak-check=full --show-reachable=yes ./shell
