#!/bin/bash 

#Validacao de argumentos
if [ -z "$1" ] || [[ $1 =~ [0-9]+$ ]] || [[ !($2 =~ [0-9]+$) ]]; then
  echo -e "\nArgumentos invalidos"
  exit
fi

#Pesquisa dentro dos dois ficheiros
pacientes=$(cat "pacientes.txt" | cut -d";" -f3 | grep -w "$1" | wc -l)
medicos=$(cat "medicos.txt" | cut -d";" -f7 | awk '$1 >'$2 | wc -l)

#Resultados
echo -e "\n-----Stats-----"
echo "Existe(m) $pacientes paciente(s) de $1."
echo "Existe(m) $medicos medico(s) com saldo superior a $2."