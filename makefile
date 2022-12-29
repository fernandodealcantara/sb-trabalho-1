all: build run

run:
	./montador -o sb/bin

build:
	g++ main.cpp pre.cpp utilitarios.cpp -o montador
