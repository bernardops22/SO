#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/*...*/
void registar_pid(){
  FILE * file = fopen("SrvConsultas.pid","a");
  fprintf(file, "%d", getpid());
  fclose(file); 
}

void trata_sinal(int sinal){
  /*...*/
}

/*...*/
int main(){
  registar_pid();
  pause();
}