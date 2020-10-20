#!/bin/bash

#Acoes iniciais
clear

#Ciclo infinito
while [ 1 ] 
do

  #Menu
  echo -e "\n-----Lista de opcoes-----"
  echo "1. Criar pacientes"
  echo "2. Cria medicos"
  echo "3. Stats"
  echo "4. Avalia medicos"
  echo -e "0. Sair\n"
  read acao
  
  #Caso nao seja inteiro
  if [[ !($acao =~ ^[0-4]$) ]]; then 
  echo -e "\nTem de digitar uma opcao valida"
  
  #Opcao 0
  elif [ $acao -eq 0 ]; then exit
  
  #Opcao 1
  elif [ $acao -eq 1 ]; then ./cria_pacientes.sh 
  
  #Opcao 2
  elif [ $acao -eq 2 ]; then 
    echo -e -n "\nNome do medico: "
    read "medico"
    echo -n "Numero de cedula profissional: "
    read numero
    echo -n "Especialidade medica: "
    read "especialidade"
    echo -n "E-mail: "
    read email
    ./cria_medico.sh "$medico" $numero "$especialidade" $email
    
  #Opcao 3
  elif [ $acao -eq 3 ]; then 
  if [ -e pacientes.txt ] && [ -e medicos.txt ]; then
    echo -n -e "\nLocalidade: "
    read "localidade"
    echo -n "Saldo: "
    read saldo
    ./stats.sh "$localidade" $saldo
  else echo -e "\nLista de medicos e/ou pacientes inexistentes. Tera de criar medicos/pacientes primeiro."
  fi
    
  #Opcao 4
  elif [ $acao -eq 4 ]; then 
  if [ -e medicos.txt ]; then
  ./avalia_medicos.sh 
  else echo -e "\nLista de medicos inexistente. Tera de criar medicos primeiro."
  fi
  fi
  
  #Finalizacao
  echo ""
  read -n 1 -s -p "Prima uma tecla para continuar"
  clear
  
done