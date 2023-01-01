#include "mcr.h"

Codigo processarMacros(const Codigo& codigo) {
  Codigo mdt, codigoSemMacros;
  MNTMap mnt;
  // gerar mnt e mdt
  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
    // se a linha tem pelo menos 2 tokens e o primeiro é um label e o segundo é ':'
    if (linha->size() >= 2 && regex_match(linha->at(0), reLabel) && linha->at(1) == ":") {
      string label = linha->at(0);  // possivel label da macro
      ++linha;                      // pula a linha do label para verificar se é uma macro
      // se for uma macro, gera a entrada na mnt e adiciona a mdt
      if (linha != codigo.end() && linha->size() >= 1 && regex_match(linha->at(0), reMacro)) {
        map<string, string> parametros;
        mnt[label].numArgs = 0;
        for (int i = 1; i < linha->size(); ++i) {
          if (regex_match(linha->at(i),
                          reMacroParam)) {  // se é um parametro da macro subtitui por #n
            parametros[linha->at(i)] = "#" + to_string(mnt[label].numArgs + 1);
            ++mnt[label].numArgs;  // incrementa o numero de parametros
          }
        }
        ++linha;                        // pula a linha da macro para analisar o corpo da macro
        mnt[label].linha = mdt.size();  // linha onde a macro começa na mdt
        while (linha != codigo.end() && linha->size() >= 1 &&
               !regex_match(linha->at(0), reEndmacro)) {  // enquanto não chega no endmacro
          LinhaCodigo linhaMacro = *linha;                // copia a linha
          for (int i = 0; i < linhaMacro.size(); ++i) {   // substitui parametros por #n
            if (parametros.find(linhaMacro[i]) != parametros.end()) {
              linhaMacro[i] = parametros[linhaMacro[i]];
            }
          }
          mdt.push_back(linhaMacro);  // adiciona a linha na mdt
          ++linha;
        }
        mdt.push_back(*linha);  // adiciona endmacro na mdt
      } else {                  // se não for uma macro, adiciona a linha no codigo sem macros
        --linha;                // desfaz o incremento da linha
        codigoSemMacros.push_back(*linha);  // adiciona a linha no codigo sem macros
      }
    } else {  // se não for uma label, adiciona a linha no codigo sem macros diretamente
      codigoSemMacros.push_back(*linha);
    }
  }

#ifdef DEBUG
  cout << "\nMDT:" << endl;
  dumpCodigo(mdt);
  cout << "\nMNT:" << endl;
  dumpMnt(mnt);
#endif

  // substituir chamadas de macros
  return subtituirMacros(codigoSemMacros, mnt, mdt);
}

Codigo subtituirMacros(const Codigo& codigo, const MNTMap& mnt, const Codigo& mdt) {
  Codigo codigoProcessado, pilhaMDT, corpoMDT;
  auto linha = codigo.begin();
  while (1) {
    while (pilhaMDT.size() > 0) {  // enquanto ha linhas na pilha de macros para processar
      // copiar topo da pilha para variavel
      LinhaCodigo linhaPilha = pilhaMDT.back();
      pilhaMDT.pop_back();
      // se a linha tem pelo menos 1 token e o primeiro é um label e o label é uma macro
      if (linhaPilha.size() >= 1 && regex_match(linhaPilha.at(0), reLabel) &&
          mnt.find(linhaPilha.at(0)) != mnt.end()) {
        corpoMDT.clear();  // limpar corpo da variavel para receber o corpo da macro

        int linhaMacro = mnt.at(linhaPilha.at(0)).linha;  // linha onde a macro começa na mdt
        // copiar corpo da macro para variavel
        while (linhaMacro < mdt.size() && !regex_match(mdt[linhaMacro][0], reEndmacro)) {
          corpoMDT.push_back(mdt[linhaMacro]);
          ++linhaMacro;
        }
        // remover virgulas dos parametros
        LinhaCodigo linhaPilhaSemVirgulas;
        for (auto token : linhaPilha) {
          if (token != ",") linhaPilhaSemVirgulas.push_back(token);
        }

        // substituir parametros
        for (int i = 0; i < corpoMDT.size(); ++i) {          // para cada linha do corpo da macro
          for (int j = 0; j < corpoMDT[i].size(); ++j) {     // para cada token da linha
            if (regex_match(corpoMDT[i][j], reParametro)) {  // se o token é um parametro
              int indexParametro = stoi(corpoMDT[i][j].substr(1));  // indice do parametro
              corpoMDT[i][j] = linhaPilhaSemVirgulas.at(indexParametro);  // substitui o parametro pelo valor
            }
          }
        }

        // empilhar corpo da macro na pilha de macros para processar
        // essa abordagem permite que macros sejam chamadas dentro de macros
        for (int i = corpoMDT.size() - 1; i >= 0; --i) {
          pilhaMDT.push_back(corpoMDT[i]);
        }

      } else {  // se não for uma chamada de macro, adiciona a linha no codigo processado
        codigoProcessado.push_back(linhaPilha);
      }
    }

    if (linha == codigo.end()) break;  // se chegou no fim do codigo, termina o loop
    // se a linha tem pelo menos 1 token e o primeiro é um label e o label é uma macro
    if (linha->size() >= 1 && regex_match(linha->at(0), reLabel) &&
        mnt.find(linha->at(0)) != mnt.end()) {
      pilhaMDT.push_back(*linha);
    } else {  // se não for uma chamada de macro, adiciona a linha no codigo processado
      codigoProcessado.push_back(*linha);
    }
    ++linha;
  }

  return codigoProcessado;
}