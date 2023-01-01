#include "analises.h"

using namespace std;

void analisarCodigo(const Codigo& codigo) {
  for (int i = 0; i < codigo.size(); i++) {
    analiseLexica(i + 1, codigo[i]);
    analiseSintatica(i + 1, codigo[i]);
  }
}

void analiseLexica(const int linha, const vector<string> tokens) {
  regex reg(uppercaseInsts + "|" + lowercaseInsts + "|" + uppercaseDirectives + "|" +
            lowercaseDirectives + "|" + uppercaseSections + "|" + lowercaseSections + "|" + labels +
            "|" + separators + "|" + values);

  for (const string& token : tokens) {
    if (!regex_match(token, reg)) {
      throw runtime_error("Erro léxico na linha " + to_string(linha) +
                          "\nCaracteres não reconhecidos:" + token);
    }
  }
}

void analiseSintatica(const int linha, const vector<string> tokens) {
  if (tokens.size() == 0) {
    return;
  }

  try {
    for (int i = 0; i < tokens.size(); i++) {
      if (regex_match(tokens[i], reSection)) {  // erro sintatico seção
        if (i + 1 >= tokens.size()) {           // seção sem argumento
          throw runtime_error("(ES:1) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i]);
        }
        if (!regex_match(tokens[i + 1], reText) &&
            !regex_match(tokens[i + 1], reData)) {  // seção com argumento inválido
          throw runtime_error("(ES:2) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 1]);
        }
      } else if (regex_match(tokens[i], reInst1Args)) {  // erro sintatico instrucao 1 arg
        if (i + 1 >= tokens.size()) {                    // instrução sem argumento
          throw runtime_error("(ES:3) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i]);
        }
        if (!regex_match(tokens[i + 1], reValues)) {  // instrução com argumento inválido
          throw runtime_error("(ES:4) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 1]);
        }
        if (i + 2 < tokens.size()) {  // instrução com argumento a mais
          throw runtime_error("(ES:3.1) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 2]);
        }
      } else if (regex_match(tokens[i], reCopy)) {  // erro sintatico copy (2 args)
        if (i + 3 >= tokens.size()) {               // copy sem argumentos
          throw runtime_error("(ES:5) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i]);
        }
        if (!regex_match(tokens[i + 1], reValues)) {  // copy com argumento inválido
          throw runtime_error("(ES:6) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 1]);
        }
        if (tokens[i + 2] != ",") {  // copy com separador de argumentos inválido
          throw runtime_error("(ES:7) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 2]);
        }
        if (!regex_match(tokens[i + 3], reValues)) {  // copy com argumento inválido
          throw runtime_error("(ES:8) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 3]);
        }
        if (i + 4 < tokens.size()) {  // copy com argumento a mais
          throw runtime_error("(ES:5.1) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 4]);
        }
      } else if (regex_match(tokens[i], reStop)) {  // erro sintatico stop (0 args)
        if (i + 1 < tokens.size()) {                // stop com argumento
          throw runtime_error("(ES:9) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 1]);
        }
      } else if (regex_match(tokens[i], reSpace)) {  // erro sintatico space (1 arg)
        if (i == 0) {                                // space sem label
          throw runtime_error("(ES:10.2) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i]);
        } else if (i + 1 < tokens.size()) {             // space com argumento
          if (!regex_match(tokens[i + 1], reValue)) {  // space com argumento inválido
            throw runtime_error("(ES:10) Erro sintático na linha " + to_string(linha) + ": " +
                                tokens[i + 1]);
          }

          if (i + 2 < tokens.size()) {  // space com argumento a mais
            throw runtime_error("(ES:10.1) Erro sintático na linha " + to_string(linha) + ": " +
                                tokens[i + 2]);
          }
        }

      } else if (regex_match(tokens[i], reConst)) {  // erro sintatico const (1 arg)
        if (i == 0) {                                // const sem label
          throw runtime_error("(ES:11.2) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i]);
        } else if (i + 1 < tokens.size()) {             // const com argumento
          if (!regex_match(tokens[i + 1], reValue)) {  // const com argumento inválido
            throw runtime_error("(ES:11) Erro sintático na linha " + to_string(linha) + ": " +
                                tokens[i + 1]);
          }

          if (i + 2 < tokens.size()) {  // const com argumento a mais
            throw runtime_error("(ES:11.1) Erro sintático na linha " + to_string(linha) + ": " +
                                tokens[i + 2]);
          }
        }
      } else if (regex_match(tokens[i], reText) ||
                 regex_match(tokens[i], reData)) {  // erro sintatico seção
        if (i - 1 < 0) {                            // seção sem diretiva
          throw runtime_error("(ES:12) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i]);
        }

        if (!regex_match(tokens[i - 1], reSection)) {  // seção sem diretiva de seção
          throw runtime_error("(ES:13) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i - 1]);
        }

      } else if (tokens[i] == ":") {  // erro sintatico dois pontos
        if (i != 1) {                 // dois pontos sem label ou com label a mais
          throw runtime_error("(ES:14) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i]);
        }
        if (!regex_match(tokens[i - 1], reLabel)) {  // dois pontos com label inválido
          throw runtime_error("(ES:15) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i - 1]);
        }
      } else if (tokens[i] == ",") {  // erro sintatico virgula
        if (i - 1 < 0) {              // virgula sem argumento
          throw runtime_error("(ES:16) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i]);
        }
        if (!regex_match(tokens[i - 1], reValues)) {  // virgula com argumento inválido
          throw runtime_error("(ES:17) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i - 1]);
        }
        if (i + 1 >= tokens.size()) {  // virgula sem argumento
          throw runtime_error("(ES:18) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 1]);
        }
        if (!regex_match(tokens[i + 1], reValues)) {  // virgula com argumento inválido
          throw runtime_error("(ES:19) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i + 1]);
        }
      } else if (regex_match(tokens[i], reLabel)) {  // erro sintatico label
        if (i == 0) {                                // label no inicio da linha
          if (i + 1 >= tokens.size()) {              // label sem resto da linha
            throw runtime_error("(ES:20) Erro sintático na linha " + to_string(linha) + ": " +
                                tokens[i]);
          }
          if (tokens[i + 1] != ":") {  // label sem dois pontos
            throw runtime_error("(ES:21) Erro sintático na linha " + to_string(linha) + ": " +
                                tokens[i + 1]);
          }
        } else {                                             // label no meio da linha
          if (!regex_match(tokens[i - 1], reBeforeValue)) {  // label sem separador de argumentos
            throw runtime_error("(ES:22) Erro sintático na linha " + to_string(linha) + ": " +
                                tokens[i - 1]);
          }
        }
      } else if (regex_match(tokens[i], reValuesNoLabel)) {  // erro sintatico argumento
        if (i - 1 < 0) {                                     // argumento no inicio da linha
          throw runtime_error("(ES:23) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i]);
        }
        if (!regex_match(tokens[i - 1], reBeforeValue)) {  // argumento sem separador de argumentos
          throw runtime_error("(ES:24) Erro sintático na linha " + to_string(linha) + ": " +
                              tokens[i - 1]);
        }
      }
    }
  } catch (runtime_error& e) {
    // erro + linha de codigo
    string erro = e.what();
    string linha_codigo = "";
    for (int i = 0; i < tokens.size(); i++) {
      linha_codigo += tokens[i] + " ";
    }
    throw runtime_error(erro + "\nErro ocorreu na linha:" + linha_codigo);
  }
}
