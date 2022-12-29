#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

int verificarArgumentos(int argc, char* argv[]);
string lerArquivo(string arquivo);
void salvarArquivo(string arquivo, map<int, vector<string>> linhas);
void dumpMap(const map<int, vector<string>>& linhas);

#endif  // UTILITARIOS_H