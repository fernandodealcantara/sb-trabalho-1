#ifndef MCR_H
#define MCR_H

#include <cstring>
#include <iostream>
#include <regex>
#include <vector>

#include "tipos.h"
#include "utilitarios.h"

using namespace std;

Codigo processarMacros(const Codigo& codigo);
Codigo subtituirMacros(const Codigo& codigo, const MNTMap& mnt, const Codigo& mdt);

#endif  // MCR_H