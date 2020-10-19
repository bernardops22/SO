#!/bin/bash 

#Lancamento de excecoes
if [ $# -ne 2 ]; then
  echo -e "\nTem de colocar 2 argumentos do tipo LOCALIDADE SALDO"
  echo "Exemplo: ./stats.sh \"Lisboa\" 500"
  exit

#Validacao de argumentos
elif [[ $1 =~ [0-9]+$ ]] || [[ !($2 =~ [0-9]+$) ]]; then
  echo -e "\nArgumentos invalidos"
  exit
fi

#Pesquisa dentro dos dois ficheiros
if [ -e pacientes.txt ] && [ -e medicos.txt ]; then
  pacientes=$(cat "pacientes.txt" | cut -d";" -f3 | grep -w "$1" | wc -l)
  medicos=$(cat "medicos.txt" | cut -d";" -f7 | awk '$1 >'$2 | wc -l)
fi

#Resultados
echo -e "\n-----Stats-----"
echo "Existem $pacientes paciente(s) que sao de $1."
echo "Existem $medicos medico(s) com saldo superior a $2."