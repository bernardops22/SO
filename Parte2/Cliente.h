#ifndef __CLIENTE_H__
#define __CLIENTE_H__

#include "header.h"

int n = 0;
Consulta c;

Consulta nova_consulta();
void pedido_consulta();
void contactar_servidor( int file );
void get_srv_pid();
void trata_sinal( int sinal );

#endif