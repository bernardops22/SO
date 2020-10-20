#!/bin/bash

#Lancamento de excecoes
if [ $(echo "${#1}") -eq 0 ] || [ $(echo "${#2}") -eq 0 ] || [ $(echo "${#3}") -eq 0 ] || [ $(echo "${#4}") -eq 0 ] || [[ $1 =~ [0-9]+$ ]] || [[ !($2 =~ [0-9]+$) ]] || [[ $3 =~ [0-9]+$ ]] || [[ !($4 == *[@]*) ]]; then
  echo -e "\nArgumentos invalidos"
  exit
fi

if [ -e medicos.txt ]; then

  #Filtro do enunciado (obrigatorio)
  ids=$(cat medicos.txt | cut -d";" -f2 | grep -w $2 | wc -l)
  if [ $ids -eq 1 ]; then 
    echo -e "\nO medico com o numero de cedula profissional $2 ja esta registado\n"
    
    #Resultados
    echo "-----Lista de medicos registados-----"
    cat medicos.txt | sort
    exit
  fi
fi

#Finalizacao
echo "$1;$2;$3;$4;0;0;0" >> medicos.txt

#Resultados
echo -e "\nMedico registado com sucesso\n"
echo "-----Lista de medicos registados-----"
cat medicos.txt | sort 