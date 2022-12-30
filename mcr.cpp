#include "mcr.h"

Codigo processarMacros(const Codigo& codigo) {
  Codigo mdt, codigoSemMacros;
  MNTMap mnt;
  // gerar mnt e mdt
  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
    if (linha->size() >= 2 && regex_match(linha->at(0), reLabel) && linha->at(1) == ":") {
      string label = linha->at(0);
      int inicioMacro = linha - codigo.begin();
      ++linha;
      if (linha->size() >= 1 && regex_match(linha->at(0), reMacro)) {
        map<string, string> parametros;
        mnt[label].numArgs = 0;
        for (int i = 1; i < linha->size(); ++i) {
          if (regex_match(linha->at(i), reMacroParam)) {
            parametros[linha->at(i)] = "#" + to_string(mnt[label].numArgs + 1);
            ++mnt[label].numArgs;
          }
        }
        ++linha;
        mnt[label].linha = mdt.size();
        while (linha != codigo.end() && linha->size() >= 1 &&
               !regex_match(linha->at(0), reEndmacro)) {
          LinhaCodigo linhaMacro = *linha;
          for (int i = 0; i < linhaMacro.size(); ++i) {
            if (parametros.find(linhaMacro[i]) != parametros.end()) {
              linhaMacro[i] = parametros[linhaMacro[i]];
            }
          }
          mdt.push_back(linhaMacro);
          ++linha;
        }
        mdt.push_back(*linha);  // adiciona endmacro
        int fimMacro = linha - codigo.begin() + 1;
      } else {
        --linha;
        codigoSemMacros.push_back(*linha);
      }
    } else {
      codigoSemMacros.push_back(*linha);
    }
  }

  // substituir chamadas de macros
  return subtituirMacros(codigoSemMacros, mnt, mdt);
}

Codigo subtituirMacros(const Codigo& codigo, const MNTMap& mnt, const Codigo& mdt) {
  Codigo codigoProcessado, pilhaMDT, corpoMDT;
  auto linha = codigo.begin();
  while (1) {
    while (pilhaMDT.size() > 0) {
      // copiar topo da pilha para variavel
      LinhaCodigo linhaPilha = pilhaMDT.back();
      pilhaMDT.pop_back();

      if (linhaPilha.size() >= 1 && regex_match(linhaPilha.at(0), reLabel) &&
          mnt.find(linhaPilha.at(0)) != mnt.end()) {
        corpoMDT.clear();

        int linhaMacro = mnt.at(linhaPilha.at(0)).linha;

        while (linhaMacro < mdt.size() && !regex_match(mdt[linhaMacro][0], reEndmacro)) {
          corpoMDT.push_back(mdt[linhaMacro]);
          ++linhaMacro;
        }

        // substituir parametros
        for (int i = 0; i < corpoMDT.size(); ++i) {
          for (int j = 0; j < corpoMDT[i].size(); ++j) {
            if (regex_match(corpoMDT[i][j], reParametro)) {
              // encontrou parametro
              // substituir parametro por valor
              int indexParametro = stoi(corpoMDT[i][j].substr(1));
              corpoMDT[i][j] = linhaPilha.at(indexParametro);
            }
          }
        }

        // empilhar corpo da macro
        for (int i = corpoMDT.size() - 1; i >= 0; --i) {
          pilhaMDT.push_back(corpoMDT[i]);
        }

      } else {
        codigoProcessado.push_back(linhaPilha);
      }
    }

    if (linha == codigo.end()) break;

    if (linha->size() >= 1 && regex_match(linha->at(0), reLabel) &&
        mnt.find(linha->at(0)) != mnt.end()) {
      pilhaMDT.push_back(*linha);
    } else {
      codigoProcessado.push_back(*linha);
    }
    ++linha;
  }

  return codigoProcessado;
}