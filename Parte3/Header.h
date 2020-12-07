#ifndef __HEADER_H__
#define __HEADER_H__

//CLIENTE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <signal.h>

#define MSGKEY 0x0a92439
#define exit_on_error(s,m) if (s<0) { perror(m); exit(1);}

//SERVIDOR

//#include <string.h>
//#include <sys/wait.h>

//#define PEDIDO_CONSULTA "PedidoConsulta.txt"
//#define SERVIDOR_PID "SrvConsultas.pid"
//#define STATS_CONSULTAS "StatsConsultas.dat"

typedef struct {
  int tipo; // Tipo de Consulta: 1-Normal, 2-COVID19, 3-Urgente
  char descricao[100]; // Descrição da Consulta
  int pid_consulta; // PID do processo que quer fazer a consulta
  int status; // Estado da consulta: 1-Pedido, 2-Iniciada, 3-Terminada, 4-Recusada, 5-Cancelada
} Consulta;

#endif