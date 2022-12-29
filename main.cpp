#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <vector>

#include "pre.h"
#include "utilitarios.h"
// O executável deve ser chamado de MONTADOR. E deve ter 3 modos de uso. Deve
// ser possível chamar por linha de comando da seguinte forma: ./montador -<op>
// <arquivo> O arquivo de entrada deve ser indicado SEM extensão e o arquivo de
// saída deve MANTER o mesmo nome e mudar a extensão. Deve existir 3 formas de
// operação: -p: A extensão da saída deve ser .PRE e somente deve se processar
// EQU e IF -m: A extensão da saída deve ser .MCR e somente deve se processar
// MACROS. A entrada vai ser a saída pré-processada -o: A extensão da saída deve
// ser OBJ e deve seguir o formato indicado anteriormente. A entrada vai ser a
// saída das Macros.

using namespace std;

map<string, int> obterOpcodesNumericos();
void analiseLexica(const int linha, const vector<string> tokens);

int main(int argc, char* argv[]) {
  auto instructions = obterOpcodesNumericos();
  verificarArgumentos(argc, argv);
  char op = argv[1][1];
  string nome_arquivo = argv[2];
  string arquivo_entrada = nome_arquivo + ".asm";
  string conteudo = lerArquivo(arquivo_entrada);

  map<int, vector<string>> linhas = processarLinhas(conteudo);

  linhas = processarEquates(linhas);
  linhas = processarIfs(linhas);
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
  // using regex
  // tokens validos são: ADD, SUB, MUL, DIV, JMP, JMPN, JMPP, JMPZ, COPY, LOAD,
  // STORE, INPUT, OUTPUT, STOP, SPACE, CONST, SECTION TEXT, SECTION DATA,
  // PUBLIC, EXTERN, EQU, IF, MACRO, ENDMACRO, BEGIN, END, ":" ",",
  // X+2(caractere+digito) e qualquer palavra que não seja uma instrução ou
  // diretiva

  // regex para tokens validos
  string uppercaseInsts = "ADD|SUB|MUL|DIV|JMP|JMPN|JMPP|JMPZ|COPY|LOAD|STORE|INPUT|OUTPUT|STOP";
  string lowercaseInsts = "add|sub|mul|div|jmp|jmpn|jmpp|jmpz|copy|load|store|input|output|stop";
  string uppercaseDirectives = "SPACE|CONST|SECTION|PUBLIC|EXTERN|EQU|IF|MACRO|ENDMACRO|BEGIN|END";
  string lowercaseDirectives = "space|const|section|public|extern|equ|if|macro|endmacro|begin|end";
  string uppercaseSections = "TEXT|DATA";
  string lowercaseSections = "text|data";
  string labels = "[A-Za-z][A-Za-z0-9_]*";
  string values = "[0-9]+|[A-Za-z]\\+[0-9]";
  string separators = ":|,";

  regex reg("((" + uppercaseInsts + ")|(" + lowercaseInsts + ")|(" + uppercaseDirectives + ")|(" +
            lowercaseDirectives + ")|(" + uppercaseSections + ")|(" + lowercaseSections + ")|(" +
            labels + ")|(" + separators + ")|(" + values + "))");

  for (const string& token : tokens) {
    if (!regex_match(token, reg)) {
      throw runtime_error("Erro léxico na linha " + to_string(linha) + ": " + token);
    }
  }
}
