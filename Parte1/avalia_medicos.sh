#!/bin/bash

#Acoes inicias
rm -f lista_negra_medicos.txt

if [ -e medicos.txt ]; then

  #Ciclo de pesquisa
  cat medicos.txt | while read line
  do
    numeroconsultas=$(echo $line | cut -d";" -f6)
    rating=$(echo $line | cut -d";" -f5)
    
    #Especificacao dos filtros
    if [ $numeroconsultas -gt 6 ] && [ $rating -lt 5 ]; then 
      nome=$(echo $line | cut -d";" -f1)
      echo $line >> lista_negra_medicos.txt
    fi
  done
  
  #Resultados
  echo -e "\n-----Lista negra de medicos na plataforma-----"
  cat lista_negra_medicos.txt
fi