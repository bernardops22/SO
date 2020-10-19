#!/bin/bash

#Acoes iniciais
rm -f pacientes.txt
i=1

#Pesquisa e recurso a filtros
cat /etc/passwd | grep "^a" | grep ":1006" | head -10 | while read line 
do
  identificacao=$(echo $line | cut -d":" -f1 | sed "s/a//")
  nome=$(echo $line | cut -d":" -f5 | sed "s/,,,//")
  echo $identificacao";"$nome";;;a"$identificacao"@iscte-iul.pt;100" >> pacientes.txt
  i=$(( $i+1 ))
done

#Resultados (opcional)
echo -e "\n-----Lista de pacientes-----"
cat pacientes.txt