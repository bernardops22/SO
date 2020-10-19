#!/bin/bash

#Lancamento de excecoes
if [ $# -ne 4 ]; then
  echo -e "\nTem de colocar 4 argumentos"
  echo -e "Exemplo: ./cria_medico.sh \"Ana Correia\" 12345 \"dentista\" aguiar@gmail.com"
  exit
fi

            #Lancar excecao quando NOME e ESPECIALIDADE contiverem integers, quando ID não for integer e quando email nao contiver "@"

if [ -e medicos.txt ]; then
  if grep -q $2 medicos.txt; then 
  
    #Excecao especifica (mandatory)
    echo -e "\nO medico com o numero de cedula profissional $2 ja esta registado\n"
    
    #Resultados (quando o script termina é obrigatorio apresentar resultados ainda que nenhum medico seja registado)
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