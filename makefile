op=-o
filename=sb/bin

all: build run

run:
	./montador $(op) $(filename)

build:
	g++ main.cpp pre.cpp mcr.cpp utilitarios.cpp tipos.cpp -o montador
