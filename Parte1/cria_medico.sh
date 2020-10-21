#!/bin/bash

#Lancamento de excecoes
if [ -z "$1" ] || [ -z "$3" ] || [[ $1 =~ [0-9]+$ ]] || [[ !($2 =~ [0-9]+$) ]] || [[ $3 =~ [0-9]+$ ]] || [[ !($4 =~ [@]) ]]; then
  echo -e "\nArgumentos invalidos"
  exit
fi

if [ -e medicos.txt ]; then
  #Filtro do enunciado (obrigatorio)
  ids=$(cat medicos.txt | cut -d";" -f2 | grep -w $2 | wc -l)
  if [ $ids -ne 0 ]; then 
    echo -e "\nO medico com o numero de cedula profissional $2 ja esta registado"
    exit
  fi
fi

#Finalizacao
echo "$1;$2;$3;$4;0;0;0" >> medicos.txt

#Resultados
echo -e "\nMedico registado com sucesso\n"
echo "-----Lista de medicos registados-----"
cat medicos.txt | sort 