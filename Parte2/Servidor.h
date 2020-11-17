#ifndef __SERVIDOR_H__
#define __SERVIDOR_H__
#include "header.h"

Consulta lista_consultas[10];
#define PID_MAX sizeof(int)*5

//S1
int n = 0;
int cperdidas = 0;
int cnormal = 0;
int ccovid19 = 0;
int curgente = 0;
int indice_da_lista = 0;

void registar_pid();
void nova_consulta();
void trata_sinal();
void limpar_lista_consultas( Consulta lista_consultas[] );
void verificar_vagas( Consulta c );
void inserir_consulta ( Consulta c );
void iniciar_consulta(Consulta c );

#endif