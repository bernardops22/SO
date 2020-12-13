#include "Header.h"

#define PID getpid ()

Consulta c;
int mq_id;
int n = 0;

void iniciar_cliente ();   //C1
void nova_consulta ();     //C1 C2
void pedido_consulta ();   //C2
void receber_mensagem ();  //C3
void tratar_mensagem ();   //C4 C5 C6
void armar_SIGINT ();      //C7 
void cancelar_pedido ();   //C7