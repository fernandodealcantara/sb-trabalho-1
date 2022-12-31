#ifndef OBJ_H
#define OBJ_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "tipos.h"
#include "utilitarios.h"
using namespace std;

vector<string> gerarCodigoObjeto(const Codigo &codigo);
map<string, int> obterOpcodesNumericos();

vector<string> obterTokens(const Codigo &codigo);

#endif  // OBJ_H