#include "mcr.h"

LinhaMap processarMacros(LinhaMap& linhas) {
  LinhaMap mdt;
  MNTMap mnt;

  gerarMntMdt(linhas, mnt, mdt);

  LinhaMap linhasProcessadas = substituirMacros(linhas, mdt, mnt);

  return linhasProcessadas;
}

void gerarMntMdt(LinhaMap& linhas, MNTMap& mnt, LinhaMap& mdt) {
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
      map<string, string> parametros;
      // salvar parametros da macro na MNT e contar numero de parametros
      mnt[nomeMacro].numArgs = 0;
      for (int i = 3; i < tokens.size(); ++i) {
        if (regex_match(tokens[i], reMacroParam)) {
          parametros[tokens[i]] = "#" + to_string(mnt[nomeMacro].numArgs + 1);
          ++mnt[nomeMacro].numArgs;
        }
      }
      // salvar conteudo da macro na MDT
      ++it;
      mnt[nomeMacro].linha = it->first;  // salvar inicio da macro na MNT
      // enquanto não for final da macro
      while (it != linhas.end() && !regex_match(it->second[0], reEndmacro)) {
        // substituir parametros por numeros
        vector<string> tokensMacro = it->second;
        for (int i = 0; i < tokensMacro.size(); ++i) {
          if (parametros.find(tokensMacro[i]) != parametros.end()) {
            tokensMacro[i] = parametros[tokensMacro[i]];
          }
        }
        mdt[it->first] = tokensMacro;
        ++it;
      }
      mdt[it->first] = it->second;  // salvar final da macro na MDT (endmacro)
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
        map<string, string> parametros;
        // salvar parametros da macro na MNT e contar numero de parametros
        mnt[nomeMacro].numArgs = 0;
        for (int i = 1; i < tokens.size(); ++i) {
          if (regex_match(tokens[i], reMacroParam)) {
            parametros[tokens[i]] = "#" + to_string(mnt[nomeMacro].numArgs + 1);
            ++mnt[nomeMacro].numArgs;
          }
        }
        // salvar conteudo da macro na MDT
        ++it;
        mnt[nomeMacro].linha = it->first;  // salvar inicio da macro na MNT
        // enquanto não for final da macro
        while (it != linhas.end() && !regex_match(it->second[0], reEndmacro)) {
          // substituir parametros por numeros
          vector<string> tokensMacro = it->second;
          for (int i = 0; i < tokensMacro.size(); ++i) {
            if (parametros.find(tokensMacro[i]) != parametros.end()) {
              tokensMacro[i] = parametros[tokensMacro[i]];
            }
          }
          mdt[it->first] = tokensMacro;
          ++it;
        }
        mdt[it->first] = it->second;  // salvar final da macro na MDT (endmacro)
        int fimMacro = it->first;
        // remover macro da lista de linhas
        for (int i = inicioMacro; i <= fimMacro; ++i) {
          linhas.erase(i);
        }
      }
    }
  }
}

LinhaMap substituirMacros(LinhaMap& linhas, LinhaMap& mdt, MNTMap& mnt) {
  LinhaMap linhasProcessadas, linhasReprocessadas;
  // substituir chamada de macro por chamada
  int indexLinhaProcessada = 1, indexLinhaReprocessada = 1;
  int indexInicioInsercao;
  int terminou_processamento = 0;
  // pegar primeira linha da lista de linhas
  auto it = linhas.begin();
  LinhaMap::iterator it2;

  while (!terminou_processamento) {
    if (it != linhas.end()) {
      indexInicioInsercao = indexLinhaProcessada;
      vector<string> tokens = it->second;

      // procurar macros
      // substituir macros por chamadas
      // Teste: Macro1 1, 2
      if (tokens.size() >= 3 && regex_match(tokens[0], reLabel) && tokens[1] == ":" &&
          regex_match(tokens[2], reLabel) && mnt.find(tokens[2]) != mnt.end()) {
        // encontrou macro
        // substituir chamadas por conteudo da MDT
        string nomeMacro = tokens[2];      // LABEL1: MACRO1 1, 2
        int linha = mnt[nomeMacro].linha;  // pega linha da macro na MDT

        // salvar label
        linhasProcessadas[indexLinhaProcessada++] = {tokens[0], tokens[1]};

        while (!regex_match(mdt[linha][0], reEndmacro)) {
          vector<string> tokensMacro = mdt[linha];
          // substituir parametros da macro
          for (int i = 0; i < tokensMacro.size(); ++i) {
            if (regex_match(tokensMacro[i], reParametro)) {
              // encontrou parametro
              // substituir parametro por valor
              int indexParametro = stoi(tokensMacro[i].substr(1));
              tokensMacro[i] = tokens[indexParametro + 2];
            }
          }
          linhasProcessadas[indexLinhaProcessada] = tokensMacro;
          ++indexLinhaProcessada;
          ++linha;
        }
      } else if (tokens.size() >= 1 && regex_match(tokens[0], reLabel) &&
                 mnt.find(tokens[0]) != mnt.end()) {
        // encontrou macro
        // substituir chamadas por conteudo da MDT
        string nomeMacro = tokens[0];      // MACRO1 ONE, DOIS
        int linha = mnt[nomeMacro].linha;  // pega linha da macro na MDT

        while (!regex_match(mdt[linha][0], reEndmacro)) {
          vector<string> tokensMacro = mdt[linha];
          // substituir parametros da macro
          for (int i = 0; i < tokensMacro.size(); ++i) {
            if (regex_match(tokensMacro[i], reParametro)) {
              // encontrou parametro
              // substituir parametro por valor
              int indexParametro = stoi(tokensMacro[i].substr(1));
              tokensMacro[i] = tokens[indexParametro];
            }
          }
          linhasProcessadas[indexLinhaProcessada] = tokensMacro;
          ++indexLinhaProcessada;
          ++linha;
        }
      } else {
        linhasProcessadas[indexLinhaProcessada] = tokens;
        ++indexLinhaProcessada;
      }
      // ir para proxima linha
      ++it;

    } else {
      terminou_processamento = 1;
    }
    // verificar se existe macro nas linhas processadas, se sim, deve processar os macros novamente
    // substituir chamadas por conteudo da MDT

    it2 = linhasProcessadas.find(indexInicioInsercao);

    while (it2 != linhasProcessadas.end() && !terminou_processamento) {
      vector<string> tokens = it2->second;

      // procurar macros
      // substituir macros por chamadas
      // Teste: Macro1 1, 2
      if (tokens.size() >= 3 && regex_match(tokens[0], reLabel) && tokens[1] == ":" &&
          regex_match(tokens[2], reLabel) && mnt.find(tokens[2]) != mnt.end()) {
        // encontrou macro
        // substituir chamadas por conteudo da MDT
        string nomeMacro = tokens[2];      // LABEL1: MACRO1 1, 2
        int linha = mnt[nomeMacro].linha;  // pega linha da macro na MDT

        // salvar label
        linhasReprocessadas[indexLinhaReprocessada++] = {tokens[0], tokens[1]};

        while (!regex_match(mdt[linha][0], reEndmacro)) {
          vector<string> tokensMacro = mdt[linha];
          // substituir parametros da macro
          for (int i = 0; i < tokensMacro.size(); ++i) {
            if (regex_match(tokensMacro[i], reParametro)) {
              // encontrou parametro
              // substituir parametro por valor
              int indexParametro = stoi(tokensMacro[i].substr(1));
              tokensMacro[i] = tokens[indexParametro + 2];
            }
          }
          linhasReprocessadas[indexLinhaReprocessada] = tokensMacro;
          ++indexLinhaReprocessada;
          ++linha;
        }
      } else if (tokens.size() >= 1 && regex_match(tokens[0], reLabel) &&
                 mnt.find(tokens[0]) != mnt.end()) {
        // encontrou macro
        // substituir chamadas por conteudo da MDT
        string nomeMacro = tokens[0];      // MACRO1 ONE, DOIS
        int linha = mnt[nomeMacro].linha;  // pega linha da macro na MDT

        while (!regex_match(mdt[linha][0], reEndmacro)) {
          vector<string> tokensMacro = mdt[linha];
          // substituir parametros da macro
          for (int i = 0; i < tokensMacro.size(); ++i) {
            if (regex_match(tokensMacro[i], reParametro)) {
              // encontrou parametro
              // substituir parametro por valor
              int indexParametro = stoi(tokensMacro[i].substr(1));
              tokensMacro[i] = tokens[indexParametro];
            }
          }
          linhasReprocessadas[indexLinhaReprocessada] = tokensMacro;
          ++indexLinhaReprocessada;
          ++linha;
        }
      } else {
        linhasReprocessadas[indexLinhaReprocessada] = tokens;
        ++indexLinhaReprocessada;
      }
      ++it2;
    }
  }

  return linhasReprocessadas;
}