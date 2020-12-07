#include "Header.h"

#define PID getpid ()
#define MAILBOX_UM 1

Consulta c;

int n = 0;

void iniciar_cliente ();         //C1
void nova_consulta ();           //C1 C2
void pedido_consulta ();         //C2
void receber_mensagens ();
void tratar_mensagem ();
void armar_SIGINT ();            //C7
void trata_SIGINT ( int sinal ); //C7