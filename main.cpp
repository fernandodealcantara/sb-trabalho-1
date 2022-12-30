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

  if (op == 'm') {
    salvarArquivo(nome_arquivo + ".mcr", linhas);
    // return 0;
  }

  // analise lexica
  for (auto it = linhas.begin(); it != linhas.end(); ++it) {
    analiseLexica(it->first, it->second);
  }

  dumpMap(linhas);

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
