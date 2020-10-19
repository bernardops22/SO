#!/bin/bash

#Acoes iniciais
clear
acao=5 

#Ciclo "infinito"
while [ $acao -ne 0 ] 
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
  #if [ $acao NAO E INTEIRO ]; then echo -e "\n Tem de digitar um inteiro"
  
  #Opcao 0
  if [ $acao -eq 0 ]; then exit
  
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
    echo -n -e "\nLocalidade: "
    read "localidade"
    echo -n "Saldo: "
    read saldo
    ./stats.sh "$localidade" $saldo
    
  #Opcao 4
  elif [ $acao -eq 4 ]; then ./avalia_medicos.sh 
  
  #Outras opcoes
  else echo -e "\nEssa opcao nao existe"
  fi
  
  #Finalizacao
  echo -e "\nPrima qualquer tecla para continuar."
  read tecla
  clear
  
done
