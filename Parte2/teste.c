#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int n = 0;
void trata_alrm(int sinal){
	printf("Recebi sinal %d\n", sinal);
	n = 1;
}

void trata_int(int sinal){
	printf("\nVou terminar...\n");
	n = 1;
}

int main() {
	printf ("esperar 5 segundos...\n");
	signal ( SIGALRM, trata_alrm );
	signal ( SIGINT, trata_int );
	alarm (5);
	while ( n == 0 )
		pause();
	printf ("ok\n");
	return 0;
}