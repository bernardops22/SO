#!/bin/bash
#Acoes iniciais
rm -f pacientes.txt
i=0
NUM_MIN_PAC=10
SALDO_MIN=100

#Pesquisa e recurso a filtros
cat /etc/passwd | grep "^a" | while read line 
do
  if [ $i -lt $NUM_MIN_PAC ]; then
    identificacao=$(echo $line | cut -d":" -f1 | sed "s/a//")
    idnumbers=$(awk -F '[0-9]' '{print NF-1}' <<< "$identificacao") #ID com 5 numeros
    idlength=$(echo "${#identificacao}") #ID com 5 caracteres
  
    #Filtro que mantem users do tipo aYYYYY com Y um inteiro
    if [[ $idnumbers -eq 5 ]] && [[ $idlength -eq 5 ]]; then
      nome=$(echo $line | cut -d":" -f5 | sed "s/,,,//")
      echo $identificacao";"$nome";;;a"$identificacao"@iscte-iul.pt;"$SALDO_MIN >> pacientes.txt
      i=$(($i+1))
    fi
  fi
done

#Resultados (opcional)
echo -e "\nLista de pacientes criada com sucesso!"
echo -e "\n-----Lista de pacientes-----"
cat pacientes.txt | sort