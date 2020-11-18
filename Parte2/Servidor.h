#ifndef __SERVIDOR_H__
#define __SERVIDOR_H__
#include "header.h"

Consulta lista_consultas[10];
Consulta c;

#define PID_MAX sizeof(int)*5

int n = 0;
int cperdidas = 0;   //S1
int cnormal = 0;     //S1
int ccovid19 = 0;    //S1
int curgente = 0;    //S1
int indice_da_lista = 1; 

void limpar_lista_consultas (); //S1
void registar_pid ();           //S2
void armar_sinal ();            //S3 S4
void trata_sinal ();            //S3 S4
void nova_consulta ();          //S3.1 S3.2
void verificar_vagas ();        //S3.3
void inserir_consulta ();       //S3.4
void iniciar_consulta ();       //S3.4 S3.4.1
void desligar_servidor ();      //S4

#endif