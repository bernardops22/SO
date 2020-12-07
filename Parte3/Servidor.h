#include "Header.h"

#define PID_MAX sizeof(int)*5

Consulta lista_consultas[10];
Consulta c;

int n = 0;
int cperdidas = 0;   //S1
int cnormal = 0;     //S1
int ccovid19 = 0;    //S1
int curgente = 0;    //S1
int indice_da_lista; 

void iniciar_servidor ();
void limpar_lista_consultas (); //S1
void registar_pid ();           //S2
void armar_sinal ();            //S3 S4
void trata_sinal ();            //S3 S4
void nova_consulta ();          //S3.1 S3.2
void verificar_vagas ();        //S3.3
void inserir_consulta ();       //S3.4
void iniciar_consulta ();       //S3.4 S3.4.1
void temporizador();
void desligar_servidor ();      //S4