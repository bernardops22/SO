#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

typedef struct {
  int tipo; // Tipo de Consulta: 1-Normal, 2-COVID19, 3-Urgente
  char descricao[100]; // Descrição da Consulta
  int pid_consulta; // PID do processo que quer fazer a consulta
} Consulta;

#endif