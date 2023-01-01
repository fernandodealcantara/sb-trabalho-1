#ifndef OBJ_H
#define OBJ_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "tipos.h"
#include "utilitarios.h"
using namespace std;

map<string, int> obterOpcodesNumericos();

vector<string> processarObjeto(const Codigo &codigo);
TabSimbolos gerarTabelaSimbolos(const Codigo &codigo);

#endif  // OBJ_H