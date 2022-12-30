#ifndef PRE_H
#define PRE_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "tipos.h"

using namespace std;

LinhaMap processarEquates(const LinhaMap& linhas);
LinhaMap processarIfs(const LinhaMap& linhas);
LinhaMap processarLinhas(string conteudo);
vector<string> tokenize(const string& str);

#endif  // PRE_H