#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include <sys/sem.h>

#define KEY 0x0a92439
#define exit_on_error(s,m) if (s<0) { perror(m); exit(1);}
#define PEDIDO 1
#define INICIADA 2
#define TERMINADA 3
#define RECUSADA 4
#define CANCELADA 5
#define TAMANHOCONSULTA 100+sizeof(int)*2+2

typedef struct {
  int tipo; // Tipo de Consulta: 1-Normal, 2-COVID19, 3-Urgente
  char descricao[100]; // Descrição da Consulta
  int pid_consulta; // PID do processo que quer fazer a consulta
  int status; // Estado da consulta: 1-Pedido, 2-Iniciada, 3-Terminada, 4-Recusada, 5-Cancelada
} Consulta;

typedef struct { 
    long tipo;
    char texto[TAMANHOCONSULTA];
} mensagem;

#endif