#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "tipos.h"

using namespace std;

int verificarArgumentos(int argc, char* argv[]);
string lerArquivo(string arquivo);
void salvarArquivo(string arquivo, const LinhaMap& linhas);
void dumpMap(const LinhaMap& linhas);
void dumpMnt(const MNTMap& mnt);

#endif  // UTILITARIOS_H