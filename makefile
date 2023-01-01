op=-o
filename=sb/bin

all: build run

run:
	./montador $(op) $(filename)

build:
	g++ main.cpp pre.cpp mcr.cpp obj.cpp analises.cpp utilitarios.cpp tipos.cpp -o montador

debug:
	g++ main.cpp pre.cpp mcr.cpp obj.cpp analises.cpp utilitarios.cpp tipos.cpp -o montador -DDEBUG