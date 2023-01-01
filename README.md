# sb-trabalho-1

Trabalho Prático 1 - Software Básico

## Grupo
Fernando de Alcantara, 190125586
Rodrigo Pereira Couto, 190116510 

## Compilação
Para compilar o programa, basta executar o comando `make build` na raiz do projeto.

Para compilar o programa em debug, basta executar o comando `make debug` na raiz do projeto. 
A compilação em debug irá mostrar mensagens de todos os passos do programa no terminal.

## Execução
Para executar o programa, basta executar o comando `make run op=<op> file=<file>` na raiz do projeto.
Onde `<op>` é a operação a ser realizada, podendo ser `-p`, `-m` ou `-o`. Note o uso do hífen antes da opção.
E `<file>` é o caminho do arquivo a ser utilizado (não informar a extensão do arquivo).

Também é possível rodar o montador diretamente:
./montador <op> <file>

## Exemplo
### Execução
make build
make run op=-p file=bin
### Execução em debug
make debug
make run op=-p file=bin

## Informações
- Sistema operacional utilizado: Ubuntu 22.04.1 LTS
- Compilador utilizado: g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
