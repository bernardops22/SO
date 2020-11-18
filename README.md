# Cliniq-IUL

O presente trabalho visa aplicar os conhecimentos adquiridos durante as aulas de Sistemas Operativos e será composto por três partes, com o objetivo de desenvolver os diferentes aspetos da plataforma **Cliniq-IUL**.

A plataforma **Cliniq-IUL** destina-se à marcação e fornecimento de consultas médicas online. Na Plataforma **Cliniq-IUL**, existem os seguintes conceitos:

• **Paciente**: a pessoa que precisa de uma consulta médica de uma determinada especialidade (ou clínica geral). O paciente tem um determinado perfil – número de identificação, nome, localidade, número de telemóvel, email, e saldo disponível para pagamento das consultas pedidas. 

• **Médico**: a pessoa que efetua a consulta de especialidade (ou clínica geral). Um médico tem um determinado perfil – Nome, número de cédula profissional, especialidade médica, email, rating como médico (de 0 a 10, atribuído pelo paciente), número de consultas efetuado, e saldo do médico recebido pelas consultas efetuadas. 

• **Pedido de Consulta**: mensagem enviada pelo Paciente a marcar uma Consulta para um determinado médico. 

• **Consulta**: Consulta de um Paciente pelo Médico com uma determinada duração e com um determinado custo (preço da consulta).

Em termos gerais, quando um Paciente precisa de uma Consulta, utiliza o seu telemóvel para se ligar ao servidor Tigre e requisitar um Médico disponível. Os médicos ativos (disponíveis) estão ligados ao servidor Tigre a aguardar pedidos de consulta.
