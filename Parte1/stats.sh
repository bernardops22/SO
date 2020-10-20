#!/bin/bash 

#Validacao de argumentos
if [ $(echo "${#1}") -eq 0 ] || [ $(echo "${#2}") -eq 0 ] || [[ $1 =~ [0-9]+$ ]] || [[ !($2 =~ [0-9]+$) ]]; then
  echo -e "\nArgumentos invalidos"
  exit
fi

#Pesquisa dentro dos dois ficheiros
#if [ -e pacientes.txt ] && [ -e medicos.txt ]; then
  pacientes=$(cat "pacientes.txt" | cut -d";" -f3 | grep -w "$1" | wc -l)
  medicos=$(cat "medicos.txt" | cut -d";" -f7 | awk '$1 >'$2 | wc -l)
#fi

#Resultados
echo -e "\n-----Stats-----"
echo "Existe(m) $pacientes paciente(s) de $1."
echo "Existe(m) $medicos medico(s) com saldo superior a $2."