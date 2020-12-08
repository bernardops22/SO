#include "Header.h"

#define PID_MAX sizeof(int)*5
#define DURACAO 10

Consulta c;
int shm_id;
int mq_id;
int n = 0;
int cperdidas = 0;   //S1
int cnormal = 0;     //S1
int ccovid19 = 0;    //S1
int curgente = 0;    //S1
//int indice_da_lista; 

void iniciar_servidor ();
void limpar_lista_consultas ();
//void registar_pid ();
void armar_SIGINT ();
void trata_SIGINT ();
void receber_pedido ();
void tratar_pedido ();
//void nova_consulta ();
void verificar_vagas ();
void inserir_consulta ();
//void iniciar_consulta ();
//void temporizador();
void desligar_servidor ();