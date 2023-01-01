#ifndef ANALISES_H
#define ANALISES_H

#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "tipos.h"
#include "utilitarios.h"

using namespace std;

void analiseLexica(const int linha, const vector<string> tokens);
void analiseSintatica(const int linha, const vector<string> tokens);

#endif  // ANALISES_H