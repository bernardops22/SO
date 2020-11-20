#include "header.h"

#define PID getpid ()

Consulta c;

int n = 0;

void iniciar_cliente ();
void nova_consulta ();                     //C1 C2
void pedido_consulta ();                   //C2 C8
void get_srv_pid ();                       //C3
void contactar_servidor ( int srv_pid );   //C3
void armar_sinal ();                       //C4 C5 C6 C7 C8
void trata_sinal ( int sinal );            //C4 C5 C6 C7 C8