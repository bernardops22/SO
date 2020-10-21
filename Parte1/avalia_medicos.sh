#!/bin/bash
#Acoes inicias
rm -f lista_negra_medicos.txt

  #Ciclo de pesquisa
  cat medicos.txt | while read line
  do
    numeroconsultas=$(echo $line | cut -d";" -f6)
    rating=$(echo $line | cut -d";" -f5)
    
    #Especificacao dos filtros
    if [ $numeroconsultas -gt 6 ] && [ $rating -lt 5 ]; then 
      echo $line >> lista_negra_medicos.txt
    fi
  done
  
  #Resultados
  if [ -e lista_negra_medicos.txt ]; then
    echo -e "\n-----Lista negra de medicos na plataforma-----"
    cat lista_negra_medicos.txt
  else echo -e "\nNao existem medicos na lista negra"
  fi