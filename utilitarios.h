#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

int verificarArgumentos(int argc, char* argv[]);
string lerArquivo(string arquivo);

#endif  // UTILITARIOS_H