op=-o
file=asm_testes/bin

all: build run

run:
	./montador $(op) $(file)

build:
	g++ main.cpp src/pre.cpp src/mcr.cpp src/obj.cpp src/analises.cpp src/utilitarios.cpp src/tipos.cpp -o montador

debug:
	g++ main.cpp src/pre.cpp src/mcr.cpp src/obj.cpp src/analises.cpp src/utilitarios.cpp src/tipos.cpp -o montador -DDEBUG
	