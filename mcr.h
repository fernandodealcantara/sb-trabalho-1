#ifndef MCR_H
#define MCR_H

#include <iostream>
#include <regex>
#include <vector>

#include "tipos.h"
#include "utilitarios.h"

using namespace std;

void gerarMntMdt(LinhaMap& linhas, MNTMap& mnt, LinhaMap& mdt);
LinhaMap substituirMacros(LinhaMap& linhas, LinhaMap& mdt, MNTMap& mnt);
LinhaMap processarMacros(LinhaMap& linhas);

#endif  // MCR_H