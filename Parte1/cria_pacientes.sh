#!/bin/bash

#Acoes iniciais
rm -f pacientes.txt
i=1

#Pesquisa e recurso a filtros
cat /etc/passwd | grep "^a" | grep ":1006" | head -10 | while read line 
do
  identificacao=$(echo $line | cut -d":" -f1 | sed "s/a//")
  idnumber="${identificacao//[^[:digit:]]/}"
  idnumber=$(echo "${#idnumber}")
  
  #Filtro que mantem users do tipo aYYYYY com Y um inteiro
  if [ $idnumber -eq 5 ]; then
    nome=$(echo $line | cut -d":" -f5 | sed "s/,,,//")
    echo $identificacao";"$nome";;;a"$identificacao"@iscte-iul.pt;100" >> pacientes.txt
    i=$(( $i+1 ))
  fi
done

#Resultados (opcional)
echo -e "\n-----Lista de pacientes-----"
cat pacientes.txt | sort