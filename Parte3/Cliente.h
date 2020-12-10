#include "Header.h"

#define PID getpid ()

Consulta c;
int mq_id;
int n = 0;

void iniciar_cliente ();
void nova_consulta ();
void pedido_consulta ();
void receber_mensagem ();
void tratar_mensagem ();
void armar_SIGINT ();
void cancelar_pedido ();