#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <vector>

#include "mcr.h"
#include "obj.h"
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

void analiseLexica(const int linha, const vector<string> tokens);
void analiseSintatica(const int linha, const vector<string> tokens, const int posicao_secao_dados);

int main(int argc, char* argv[]) {
  auto instructions = obterOpcodesNumericos();
  verificarArgumentos(argc, argv);
  char op = argv[1][1];
  string nome_arquivo = argv[2];
  string conteudo = lerArquivo(nome_arquivo + ".asm");

  Codigo codigo = processarLinhas(conteudo);

  codigo = processarEquates(codigo);
  codigo = processarIfs(codigo);

  if (op == 'p') {
    salvarArquivo(nome_arquivo + ".pre", codigo);
    // return 0;
  }

  codigo = processarMacros(codigo);
  gerarCodigoObjeto(codigo);
  if (op == 'm') {
    salvarArquivo(nome_arquivo + ".mcr", codigo);
    // return 0;
  }

  // pegar posição da seção de dados
  int posicao_secao_dados = 0;
  for (int i = 0; i < codigo.size(); i++) {
    if (codigo[i].size() >= 2 && regex_match(codigo[i][0], reSection) &&
        regex_match(codigo[i][1], reData)) {
      posicao_secao_dados = i;
      break;
    }
  }

  for (int i = 0; i < codigo.size(); i++) {
    analiseLexica(i + 1, codigo[i]);
    analiseSintatica(i + 1, codigo[i], posicao_secao_dados);
  }
  // vector<string> tokens = obterTokens(codigo);
  // analiseLexica(0, codigo);
  // dumpLinhaCodigo(tokens);

  // gerarCodigoObjeto(codigo);

  // analise lexica
  // for (auto it = linhas.begin(); it != linhas.end(); ++it) {
  //   analiseLexica(it->first, it->second);
  // }

  dumpCodigo(codigo);

  return 0;
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

void analiseSintatica(const int linha, const vector<string> tokens, const int posicao_secao_dados) {
  if (tokens.size() == 0) {
    return;
  }

  regex reInst1Args(uppercaseInsts1Arg + "|" + lowercaseInsts1Arg);
  regex reValues(values + "|" + labels);

  for (int i = 0; i < tokens.size(); i++) {
    if (regex_match(tokens[i], reSection)) {  // erro sintatico seção
      if (i + 1 >= tokens.size()) {
        throw runtime_error("Erro sintático na linha (1) " + to_string(linha) + ": " + tokens[i]);
      }
      if (!regex_match(tokens[i + 1], reText) && !regex_match(tokens[i + 1], reData)) {
        throw runtime_error("Erro sintático na linha (2) " + to_string(linha) + ": " +
                            tokens[i + 1]);
      }
    } else if (regex_match(tokens[i], reInst1Args)) {  // erro sintatico instrucao 1 arg
      if (i + 1 >= tokens.size()) {
        throw runtime_error("Erro sintático na linha (3) " + to_string(linha) + ": " + tokens[i]);
      }
      if (!regex_match(tokens[i + 1], reValues)) {
        throw runtime_error("Erro sintático na linha (4) " + to_string(linha) + ": " +
                            tokens[i + 1]);
      }
    } else if (regex_match(tokens[i], reCopy)) {  // erro sintatico copy (2 args)
      if (i + 3 >= tokens.size()) {
        throw runtime_error("Erro sintático na linha (5) " + to_string(linha) + ": " + tokens[i]);
      }
      if (!regex_match(tokens[i + 1], reValues)) {
        throw runtime_error("Erro sintático na linha (6) " + to_string(linha) + ": " +
                            tokens[i + 1]);
      }
      if (tokens[i + 2] != ",") {
        throw runtime_error("Erro sintático na linha (7) " + to_string(linha) + ": " +
                            tokens[i + 2]);
      }
      if (!regex_match(tokens[i + 3], reValues)) {
        throw runtime_error("Erro sintático na linha (8) " + to_string(linha) + ": " +
                            tokens[i + 3]);
      }
    } else if (regex_match(tokens[i], reStop)) {  // erro sintatico stop (0 args)
      if (i + 1 < tokens.size()) {
        throw runtime_error("Erro sintático na linha (9) " + to_string(linha) + ": " +
                            tokens[i + 1]);
      }
    } else if (linha > posicao_secao_dados && regex_match(tokens[i], reLabel)) {
      if (tokens.size() > i + 3 && tokens[i + 1] == ":") {
        if (!regex_match(tokens[i + 2], reSpace) && !regex_match(tokens[i + 2], reConst)) {
          throw runtime_error("Erro sintático na linha (10) " + to_string(linha) + ": " +
                              tokens[i + 2]);
        }
        if (!regex_match(tokens[i + 3], reValues)) {
          throw runtime_error("Erro sintático na linha (11) " + to_string(linha) + ": " +
                              tokens[i + 3]);
        }
      }
    } else if (linha <= posicao_secao_dados && regex_match(tokens[i], reLabel)) {
      // if (tokens.size() > i + 1) {
      //   if (tokens[i + 1] == ",") {
      //     if (tokens.size() > i + 2 && !regex_match(tokens[i + 2], reValues)) {
      //       throw runtime_error("Erro sintático na linha (12) " + to_string(linha) + ": " +
      //                           tokens[i + 1]);
      //     }
      //   } else if (tokens[i + 1] != ":") {
      //     throw runtime_error("Erro sintático na linha (13) " + to_string(linha) + ": " +
      //                         tokens[i + 1]);
      //   }
      // } else if (tokens.size() == i + 1) {
      //   throw runtime_error("Erro sintático na linha (14) " + to_string(linha) + ": " +
      //                       tokens[i + 1]);
      // }
    }
  }
}
