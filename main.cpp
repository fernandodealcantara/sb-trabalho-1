#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <vector>

#include "mcr.h"
#include "pre.h"
#include "tipos.h"
#include "utilitarios.h"
// O executável deve ser chamado de MONTADOR. E deve ter 3 modos de uso. Deve
// ser possível chamar por linha de comando da seguinte forma: ./montador -<op>
// <arquivo> O arquivo de entrada deve ser indicado SEM extensão e o arquivo de
// saída deve MANTER o mesmo nome e mudar a extensão. Deve existir 3 formas de
// operação:
// -p: A extensão da saída deve ser .PRE e somente deve se processar
// EQU e IF
// -m: A extensão da saída deve ser .MCR e somente deve se processar MACROS. A entrada vai ser a
// saída pré-processada
// -o: A extensão da saída deve ser OBJ e deve seguir o formato indicado anteriormente. A entrada
// vai ser a saída das Macros.

using namespace std;

map<string, int> obterOpcodesNumericos();
void analiseLexica(const int linha, const vector<string> tokens);
LinhaMap processarMacros(LinhaMap& linhas);

int main(int argc, char* argv[]) {
  auto instructions = obterOpcodesNumericos();
  verificarArgumentos(argc, argv);
  char op = argv[1][1];
  string nome_arquivo = argv[2];
  string conteudo = lerArquivo(nome_arquivo + ".asm");

  LinhaMap linhas = processarLinhas(conteudo);

  linhas = processarEquates(linhas);
  linhas = processarIfs(linhas);

  if (op == 'p') {
    salvarArquivo(nome_arquivo + ".pre", linhas);
    // return 0;
  }

  linhas = processarMacros(linhas);

  // analise lexica
  for (auto it = linhas.begin(); it != linhas.end(); ++it) {
    analiseLexica(it->first, it->second);
  }

  // dumpMap(linhas);

  return 0;
}

map<string, int> obterOpcodesNumericos() {
  // Declare a map to store the instructions
  map<string, int> opcode;

  // Populate the map with some instructions
  opcode["ADD"] = 1;
  opcode["SUB"] = 2;
  opcode["MUL"] = 3;
  opcode["DIV"] = 4;
  opcode["JMP"] = 5;
  opcode["JMPN"] = 6;
  opcode["JMPP"] = 7;
  opcode["JMPZ"] = 8;
  opcode["COPY"] = 9;
  opcode["LOAD"] = 10;
  opcode["STORE"] = 11;
  opcode["INPUT"] = 12;
  opcode["OUTPUT"] = 13;
  opcode["STOP"] = 14;
  // Return the map
  return opcode;
}

void analiseLexica(const int linha, const vector<string> tokens) {
  regex reg("((" + uppercaseInsts + ")|(" + lowercaseInsts + ")|(" + uppercaseDirectives + ")|(" +
            lowercaseDirectives + ")|(" + uppercaseSections + ")|(" + lowercaseSections + ")|(" +
            labels + ")|(" + separators + ")|(" + values + "))");

  for (const string& token : tokens) {
    if (!regex_match(token, reg)) {
      throw runtime_error("Erro léxico na linha " + to_string(linha) + ": " + token);
    }
  }
}

LinhaMap processarMacros(LinhaMap& linhas) {
  LinhaMap linhasProcessadas, mdt;
  MNTMap mnt;

  mntMdt(linhas, mnt, mdt);

  std::cout << "Linhas " << endl;
  dumpMap(linhas);
  std::cout << "---------- MNT ----------" << endl;
  dumpMnt(mnt);
  std::cout << "---------- MDT ----------" << endl;
  dumpMap(mdt);

  // substituir chamada de macro por chamada
  int indexLinhaProcessada = 1;
  int indexInicioInsercao;
  int terminou_processamento = 0;
  // pegar primeira linha da lista de linhas
  auto it = linhas.begin();

  while (!terminou_processamento) {
    if (it != linhas.end()) {
      indexInicioInsercao = indexLinhaProcessada;
      vector<string> tokens = it->second;

      // procurar macros
      // substituir macros por chamadas
      // Teste: Macro1 1, 2
      if (tokens.size() >= 3 && regex_match(tokens[0], reLabel) && tokens[1] == ":" &&
          regex_match(tokens[2], reLabel) && mnt.find(tokens[2]) != mnt.end()) {
        cout << "Encontrou macro 3" << endl;
        // encontrou macro
        // substituir chamadas por conteudo da MDT
        string nomeMacro = tokens[2];      // LABEL1: MACRO1 1, 2
        int linha = mnt[nomeMacro].linha;  // pega linha da macro na MDT

        while (!regex_match(mdt[linha][0], reEndmacro)) {
          vector<string> tokensMacro = mdt[linha];
          // substituir parametros da macro
          for (int i = 0; i < tokensMacro.size(); ++i) {
            if (regex_match(tokensMacro[i], reMacroParam)) {
              int numParam = stoi(tokensMacro[i].substr(1));
              tokensMacro[i] = tokens[numParam + 2];
            }
          }
          linhasProcessadas[indexLinhaProcessada] = tokensMacro;
          ++indexLinhaProcessada;
          ++linha;
        }
      } else if (tokens.size() >= 1 && regex_match(tokens[0], reLabel) &&
                 mnt.find(tokens[0]) != mnt.end()) {
        cout << "Encontrou macro 1" << endl;
        // encontrou macro
        // substituir chamadas por conteudo da MDT
        string nomeMacro = tokens[0];      // MACRO1 ONE, DOIS
        int linha = mnt[nomeMacro].linha;  // pega linha da macro na MDT

        while (!regex_match(mdt[linha][0], reEndmacro)) {
          vector<string> tokensMacro = mdt[linha];
          // substituir parametros da macro
          for (int i = 0; i < tokensMacro.size(); ++i) {
            if (regex_match(tokensMacro[i], reMacroParam)) {
              if(i<tokens.size()-1){
                cout << "tokensMacro[i]: " << tokensMacro[i] << endl;
                cout << "tokens[i+1]: " << tokens[i+1] << endl;
                cout << "tokens[i]: " << tokens[i] << endl;
                tokensMacro[i] = tokens[i+1];
              }
              // encontrar indice do parametro
              int numParam;
              for (numParam = 0; numParam < tokens.size(); ++numParam) {
                if (tokens[numParam] == tokensMacro[i]) break; // one == &a
              }
              cout << "numParam: " << numParam << endl;
              cout << "tokensMacro[i]: " << tokensMacro[i] << endl;
              // cout << "tokens[numParam]: " << tokens[numParam] << endl;
              // mostrar tokens
              for (int i = 0; i < tokens.size(); ++i) {
                cout << "tokens[" << i << "]: " << tokens[i] << endl;
              }
              tokensMacro[i] = tokens[numParam+1];
            }
          }
          linhasProcessadas[indexLinhaProcessada] = tokensMacro;
          ++indexLinhaProcessada;
          ++linha;
        }
      } else {
        cout << "Nao encontrou macro" << endl;
        linhasProcessadas[indexLinhaProcessada] = tokens;
        ++indexLinhaProcessada;
      }
      // ir para proxima linha
      ++it;

    } else {
      terminou_processamento = 1;
    }
    // verificar se existe macro nas linhas processadas, se sim, repetir o processo para as linhas
    // processadas
  }

  // substituir chamadas por conteudo da MDT

  // substituir chamadas por conteudo da MDT

  std::cout << "---------- Linhas Processadas ----------" << endl;
  dumpMap(linhasProcessadas);
  return linhasProcessadas;
}