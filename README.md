# sb-trabalho-1

Trabalho Prático 1 - Software Básico

## Grupo
Fernando de Alcantara, 190125586

Rodrigo Pereira Couto, 190116510 

## Compilação
### Compilar utilizando `makefile`
Para compilar o programa, basta executar o comando `make build` na raiz do projeto.

Para compilar o programa em debug, basta executar o comando `make debug` na raiz do projeto. 
A compilação em debug irá mostrar mensagens de todos os passos do programa no terminal.

### Compilar utilizando `g++`
Também é possível compilar o montador diretamente tanto no Linux quanto no Windows:

g++ main.cpp src/pre.cpp src/mcr.cpp src/obj.cpp src/analises.cpp src/utilitarios.cpp src/tipos.cpp -o montador

Caso queira compilar o montador em debug, basta adicionar a flag `-DDEBUG` ao comando acima.

## Execução
### Executar utilizando `makefile`
Para executar o programa, basta executar o comando `make run op=<op> file=<file>` na raiz do projeto.

Onde `<op>` é a operação a ser realizada, podendo ser `-p`, `-m` ou `-o`. Note o uso do hífen antes da opção.

E `<file>` é o caminho do arquivo a ser utilizado (não informar a extensão do arquivo).

### Executar utilizando `g++`
Também é possível rodar o montador diretamente (no Windows, deve executar o arquivo montador.exe):
`./montador <op> <file>`

## Exemplo
### Execução
make build

make run op=-p file=bin

### Execução em debug
make debug

make run op=-p file=bin

### Execução direta
g++ main.cpp src/pre.cpp src/mcr.cpp src/obj.cpp src/analises.cpp src/utilitarios.cpp src/tipos.cpp -o montador

Linux: ./montador -p bin

Windows: .\montador.exe -p bin

## Informações
Linux:
- Sistema operacional utilizado: Ubuntu 22.04.1 LTS
- Compilador utilizado: g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0

Windows:
- Sistema operacional utilizado: Windows 10 Pro 22H2
- Compilador utilizado: g++.exe (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders) 11.2.0
  
