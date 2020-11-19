## Shell Script (bash)

Nesta fase do trabalho será criado um conjunto de scripts para administração e gestão do sistema.

#### **1) cria_pacientes.sh**

Este script deverá ler o ficheiro **/etc/passwd** do servidor Tigre, transformar os 10 primeiros utilizadores alunos que nele constam em “Pacientes do sistema Cliniq-IUL” e registá-los, criando um novo ficheiro (apagando um já existente) chamado **pacientes.txt**, cuja sintaxe é a seguinte: 

```
<Nº registo (nº identificação)>;<nome>;<localidade>;<nº telemóvel>;<e-mail>;<saldo>
```

Como tal, para esses dez primeiros utilizadores alunos, este script deverá:

• Extrair o campo nº de aluno (sem a letra “a”). Este campo será o nº de identificação do paciente; 

• Extrair o Nome do aluno que será o nome do paciente; 

• O e-mail deverá ser criado automaticamente tendo por base o número de identificação e acrescentando “@iscte-iul.pt”; 

• O saldo inicial de todos os pacientes criados é 100.

Exemplo de um registo completo, já com todos os campos preenchidos:

```
61999;João Silva;Lisboa;912345678;a61999@iscte-iul.pt;100
```

Os dados dos campos **localidade** e **Nº telemóvel** serão adicionados posteriormente dado que não há ainda essa informação, pelo que os campos devem ficar vazios. 

Exemplo de linhas criadas no ficheiro após a importação:

```
61999;João Silva;;;a61999@iscte-iul.pt;100
61777;Luis Passos;;;a61777@iscte-iul.pt;100
55555;Maria Aguiar;;;a55555@iscte-iul.pt;100
```

#### **2) cria_medico.sh**

Este script é executado quando se pretende inscrever um médico na plataforma. O script é executado com os seguintes argumentos: **nome, nº de cédula profissional, especialidade médica, e-mail.**

Exemplo de invocação do script, **que deverá receber os valores pedidos passados por argumentos**:

```
./cria_medico.sh "Ana Correia" 12345 "dentista" aguiar@gmail.com
./cria_medico.sh "João Vieira" 98765 "clinica geral" vieira@gmail.com
```

Com os campos inseridos como descrito acima, o script deve verificar se o médico em questão já está registado, e se não, acrescentá-lo ao ficheiro **medicos.txt**, mantendo a seguinte sintaxe:

```
<nome>;<número cédula profissional>;<especialidade médica>;<e-mail>;<rating>;<nº de consultas efetuadas>;<saldo>
```

Exemplo do ficheiro medicos.txt:

```
Ana Correia;12345;dentista;aguiar@gmail.com;0;0;0
João Vieira;54321;clinica geral;vieira@gmail.com;0;0;0
```

Nesta fase, o Rating, o nº de consultas efetuadas e o Saldo são mantidos a “0” (zero). 

**O script deve mostrar toda a lista de médicos no final da sua execução.**

#### **3) stats.sh**

Este script é executado pelo administrador do sistema para obter informações sobre o sistema. Nomeadamente, deve devolver a seguinte informação, consoante os argumentos passados:

• Número de Pacientes inscritos na Plataforma de uma **determinada localidade**; 

• Número de Médicos inscritos na Plataforma com **saldo superior a um determinado valor**.

Exemplo de invocação do script, **que deverá receber os valores pedidos passados por argumentos**:

```
./stats.sh Lisboa 500 # Exemplo com Localidade=Lisboa e Saldo mínimo=500
```

#### 4) avalia_medicos.sh

No caso de haver médicos inscritos na plataforma cujo número de consultas efetuadas seja superior a 6 e o seu rating inferior a 5, o administrador pode colocar o respetivo médico na lista negra da plataforma. Para isso, deverá existir um ficheiro “**lista_negra_medicos.txt**” onde será inserida a informação do médico (linha completa) que não cumpra com o requisito pedido.

**No final da execução do script deverá ser impressa a “lista negra” de médicos inscritos na plataforma.**

#### 5) menu

Este script agrega os restantes, mostrando um menu com as opções: 

1 - Cria pacientes 

2 - Cria médicos 

3 - Stats 

4 - Avalia médicos

0 - Sair

Cada uma das opções anteriores irá invocar o respetivo script descrito nas alíneas anteriores. No caso das opções 2 e 3, este script deverá pedir ao utilizador (administrador) as informações necessárias para execução dos scripts correspondentes, injetando as mesmas como argumentos desses scripts.

Após a execução de cada sub-script, o menu deverá voltar a ser apresentado, e nova opção pedida. Apenas a opção 0 (zero) permite sair deste script “menu”. Até escolher esta opção, o menu deverá ficar em ciclo, permitindo realizar múltiplas operações iterativamente.
