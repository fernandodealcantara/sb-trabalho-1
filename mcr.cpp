#include "mcr.h"

void mntMdt(LinhaMap& linhas, MNTMap& mnt, LinhaMap& mdt) {

  // encontrar macros e salvar na MDT e MNT
  // substituir macros por chamadas
  // substituir chamadas por conteudo da MDT
  // encontrar macros

  for (auto it = linhas.begin(); it != linhas.end(); ++it) {
    vector<string> tokens = it->second;
    // Teste: macro &A, &B, &C
    if (tokens.size() >= 3 && regex_match(tokens[0], reLabel) && tokens[1] == ":" &&
        regex_match(tokens[2], reMacro)) {
      // encontrou macro
      string nomeMacro = tokens[0];
      int inicioMacro = it->first;
      // salvar parametros da macro na MNT
      mnt[nomeMacro].numArgs = 0;
      for (int i = 3; i < tokens.size(); ++i) {
        if (regex_match(tokens[i], reMacroParam)) {
          ++mnt[nomeMacro].numArgs;
        }
      }
      // salvar conteudo da macro na MDT
      ++it;
      mnt[nomeMacro].linha = it->first;  // salvar inicio da macro na MNT
      // enquanto não for final da macro
      while (it != linhas.end() && !regex_match(it->second[0], reEndmacro)) {
        mdt[it->first] = it->second;
        ++it;
      }
      mdt[it->first] = it->second;
      int fimMacro = it->first;
      // remover macro da lista de linhas
      for (int i = inicioMacro; i <= fimMacro; ++i) {
        linhas.erase(i);
      }
    }
    // Teste: {enter} macro &A, &B, &C
    else if (tokens.size() == 2 && regex_match(tokens[0], reLabel) && tokens[1] == ":") {
      string nomeMacro = tokens[0];
      int inicioMacro = it->first;
      ++it;
      tokens = it->second;
      // macro &A, &B, &C
      if (tokens.size() >= 1 && regex_match(tokens[0], reMacro)) {
        // salvar parametros da macro na MNT
        mnt[nomeMacro].numArgs = 0;
        for (int i = 1; i < tokens.size(); ++i) {
          if (regex_match(tokens[i], reMacroParam)) {
            ++mnt[nomeMacro].numArgs;
          }
        }
        // salvar conteudo da macro na MDT
        ++it;
        mnt[nomeMacro].linha = it->first;  // salvar inicio da macro na MNT
        // enquanto não for final da macro
        while (it != linhas.end() && !regex_match(it->second[0], reEndmacro)) {
          mdt[it->first] = it->second;
          ++it;
        }
        mdt[it->first] = it->second;
        int fimMacro = it->first;
        // remover macro da lista de linhas
        for (int i = inicioMacro; i <= fimMacro; ++i) {
          linhas.erase(i);
        }
      }
    }
  }
}