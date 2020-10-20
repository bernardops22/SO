#!/bin/bash

#Acoes iniciais
rm -f pacientes.txt
i=0

#Pesquisa e recurso a filtros
cat /etc/passwd | grep "^a" | while read line 
do
  if [ $i -lt 10 ]; then
    identificacao=$(echo $line | cut -d":" -f1 | sed "s/a//")
    idnumbers="${identificacao//[^[:digit:]]/}"
    idnumbers=$(echo "${#idnumbers}") #ID com 5 numeros
    idlength=$(echo "${#identificacao}") #ID com 5 caracteres
  
    #Filtro que mantem users do tipo aYYYYY com Y um inteiro
    if [[ $idnumbers -eq 5 ]] && [[ $idlength -eq 5 ]]; then
      nome=$(echo $line | cut -d":" -f5 | sed "s/,,,//")
      echo $identificacao";"$nome";;;a"$identificacao"@iscte-iul.pt;100" >> pacientes.txt
      i=$(( $i+1 ))
    fi
  fi
done

#Resultados (opcional)
echo -e "\n-----Lista de pacientes-----"
cat pacientes.txt | sort