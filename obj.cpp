#include "obj.h"

vector<string> processarObjeto(const Codigo &codigo) {
  auto tabela_instrucoes = obterOpcodesNumericos();
  vector<string> codigo_objeto;
  bool eh_secao_data = false;
  regex reTodasInsts(uppercaseInsts + "|" + lowercaseInsts);

  if (codigo.size() == 0) {
    throw runtime_error("Erro semântico - Arquivo vazio");
  }

  // verifica se seção de texto foi declarada
  if (codigo[0].size() < 2 || !regex_match(codigo[0][0], reSection) ||
      !regex_match(codigo[0][1], reText)) {
    throw runtime_error("Erro semântico - Seção de texto não declarada");
  }

  TabSimbolos tabela_simbolos = gerarTabelaSimbolos(codigo);

  #ifdef DEBUG
  cout << "\nTabela de símbolos:" << endl;
  dumpTabSimbolos(tabela_simbolos);
  #endif

  vector<string> linha_atual;  // linha atual do código
  // Passo 2: Gerar código objeto
  try {
    for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
      linha_atual = *linha;
      if (linha->size() == 0) {  // se a linha está vazia (por algum motivo...)
        continue;
      }
      if (eh_secao_data) {         // se a seção atual é a seção de dados
        if (linha->size() == 4) {  // se a linha tem 4 tokens (label, :, const/space, valor)
          if (regex_match(linha->at(2), reConst)) {
            // se for const, pega o valor e coloca no código objeto (converter de hexa para decimal
            // se for necessário)
            codigo_objeto.push_back(to_string(stoi(linha->at(3), nullptr, 0)));
          } else {  // se for space, coloca 0 no código objeto o número de vezes especificado
            for (int i = 0; i < stoi(linha->at(3), nullptr, 0); i++) {
              codigo_objeto.push_back("0");
            }
          }
        } else if (regex_match(linha->at(2), reSpace)) {  // se for space, coloca 0 no código objeto
          codigo_objeto.push_back("0");
        }
      } else {  // se a seção atual é a seção de texto
        if (linha->size() >= 2) {
          if (regex_match(linha->at(0), reSection) && regex_match(linha->at(1), reData)) {
            eh_secao_data = true;  // se a linha é uma declaração de seção de dados, muda a seção
            continue;              // e pula para a próxima linha
          } else if (regex_match(linha->at(0), reLabel) && linha->at(1) == ":") {
            continue;  // se a linha é um label, pula para a próxima linha
          }
        }

        if (regex_match(linha->at(0), reTodasInsts)) {
          int opcode = tabela_instrucoes[linha->at(0)];
          codigo_objeto.push_back(to_string(opcode));

          for (int i = 1; i < linha->size(); i++) {    // para cada argumento da instrução
            if (regex_match(linha->at(i), reLabel)) {  // se for label
              if (tabela_simbolos.find(linha->at(i)) == tabela_simbolos.end()) {
                throw runtime_error("Erro semântico - Label não declarado");
              }
              codigo_objeto.push_back(to_string(tabela_simbolos[linha->at(i)]));
            } else if (regex_match(linha->at(i), reSymbolPlusValue)) {  // se for label + valor
              string label = linha->at(i).substr(0, linha->at(i).find("+"));
              int valor = stoi(linha->at(i).substr(linha->at(i).find("+") + 1), nullptr, 0);
              if (tabela_simbolos.find(label) == tabela_simbolos.end()) {
                throw runtime_error("Erro semântico - Label não declarado");
              }
              codigo_objeto.push_back(to_string(tabela_simbolos[label] + valor));
            } else if (regex_match(linha->at(i), reValue)) {  // se for valor
              codigo_objeto.push_back(to_string(stoi(linha->at(i), nullptr, 0)));
            }
          }
        }
      }
    }
  } catch (runtime_error &e) {
    string linha = "";
    for (auto token : linha_atual) {
      linha += token + " ";
    }
    throw runtime_error(e.what() + string("\nLinha: ") + linha);
  }

  return codigo_objeto;
}

TabSimbolos gerarTabelaSimbolos(const Codigo &codigo) {
  TabSimbolos tabela_simbolos;
  int contador_endereco = 0;
  bool eh_secao_data = false, ultima_linha_foi_label = false;
  string ultima_label_lida = "";
  regex reTodasInsts(uppercaseInsts + "|" + lowercaseInsts);
  vector<string> linha_atual;

  // Passo 1: Criar tabela de simbolos
  try {
    for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
      linha_atual = *linha;      // para mostrar a linha que deu erro
      if (linha->size() == 0) {  // se a linha está vazia (por algum motivo...)
        continue;
      }
      if (eh_secao_data) {                               // seção de dados
        if (linha->size() != 3 && linha->size() != 4) {  // se a linha não tem 3 ou 4 tokens
          throw runtime_error("Erro semântico - Seção de dados com formato inválido");
        }
        if (!regex_match(linha->at(0), reLabel)) {  // se o label não é válido
          throw runtime_error("Erro semântico - Label inválido");
        }
        if (linha->at(1) != ":") {  // se não tem separação
          throw runtime_error("Erro semântico - Separação inválida");
        }
        if (!regex_match(linha->at(2), reConst) &&
            !regex_match(linha->at(2), reSpace)) {  // se o tipo de dado não é válido
          throw runtime_error("Erro semântico - Tipo de dado inválido");
        }

        if (regex_match(linha->at(2), reConst) &&
            linha->size() != 4) {  // se é constante e não tem valor
          throw runtime_error("Erro semântico - Constante sem valor");
        }

        if (linha->size() == 4 &&
            !regex_match(linha->at(3), secDataValues)) {  // se o valor não é válido
          throw runtime_error("Erro semântico - Valor inválido");
        }
        // se o label já existe na tabela de simbolos
        if (tabela_simbolos.find(linha->at(0)) != tabela_simbolos.end()) {  // se o label já existe
          throw runtime_error("Erro semântico - Label já existe");
        }
        tabela_simbolos[linha->at(0)] = contador_endereco;
        if (regex_match(linha->at(2), reConst)) {  // se é constante
          contador_endereco += 1;                  // constante ocupa 1 espaço
        } else if (linha->size() == 4 &&
                   regex_match(linha->at(2), reSpace)) {  // se é espaço e tem valor
          // valor pode estar em formato decimal ou hexadecimal
          int valor = stoi(linha->at(3), nullptr, 0);
          if (valor < 0) {  // se o valor é negativo
            throw runtime_error("Erro semântico - Valor negativo para espaço");
          }
          contador_endereco += valor;  // espaço ocupa o valor especificado de espaços
        } else {                       // se é espaço e não tem valor
          contador_endereco += 1;      // espaço ocupa 1 espaço
        }
      } else {                     // seção de texto
        if (linha->size() >= 2) {  // se a linha tem pelo menos 2 tokens
          if (regex_match(linha->at(0), reSection) &&
              regex_match(linha->at(1), reData)) {  // achou seção de dados
            eh_secao_data = true;
            continue;
          } else if (regex_match(linha->at(0), reLabel) &&
                     linha->at(1) == ":") {  // se a linha tem label
            if (tabela_simbolos.find(linha->at(0)) != tabela_simbolos.end()) {
              throw runtime_error("Erro semântico - Label já existe");
            }
            if (ultima_linha_foi_label) {  // se a linha anterior foi label
              throw runtime_error("Erro semântico - Label sem instrução");
            }
            ultima_linha_foi_label = true;     // marca que a linha atual foi label
            ultima_label_lida = linha->at(0);  // guarda o label lido
            continue;
          }
        }
        if (regex_match(linha->at(0), reTodasInsts)) {  // se a linha eh instrucao
          if (ultima_linha_foi_label) {                 // se a linha anterior foi label
            tabela_simbolos[ultima_label_lida] = contador_endereco;
            ultima_linha_foi_label = false;
          }
          contador_endereco += 1;                        // instrução ocupa 1 espaço
          if (regex_match(linha->at(0), reInst1Args)) {  // se a instrução tem 1 argumento
            if (linha->size() != 2) {
              throw runtime_error("Erro semântico - Instrução com formato inválido");
            }
            if (!regex_match(linha->at(1), reValues)) {
              throw runtime_error("Erro semântico - Argumento inválido");
            }
            contador_endereco += 1;  // instrução com 1 argumento ocupa 1 espaço
          } else if (regex_match(linha->at(0), reCopy)) {  // se a instrução é copy (2 argumentos)
            if (linha->size() != 4) {
              throw runtime_error("Erro semântico - Instrução com formato inválido");
            }
            if (!regex_match(linha->at(1), reValues) && linha->at(2) != "," &&
                !regex_match(linha->at(3), reValues)) {
              throw runtime_error("Erro semântico - Argumento inválido");
            }
            contador_endereco += 2;  // instrução com 2 argumentos ocupa 2 espaços
          } else if (regex_match(linha->at(0), reStop)) {  // se a instrução é stop (0 argumentos)
            if (linha->size() != 1) {
              throw runtime_error("Erro semântico - Instrução não deve ter argumentos");
            }
          }
        }
      }
    }
  } catch (runtime_error &e) {
    // se houve erro, relançar a exceção com a linha que deu erro
    string linha_string = "";
    for (auto token = linha_atual.begin(); token != linha_atual.end(); ++token) {
      linha_string += *token + " ";
    }
    throw runtime_error(e.what() + string("\nNa linha: ") + linha_string);
  }

  return tabela_simbolos;
}

vector<string> gerarCodigoObjeto(const Codigo &codigo) {
  map<string, int> opcode = obterOpcodesNumericos();
  // vetor de dos objetos
  vector<string> objetos;
  // map tabela de simbolos
  map<string, int> tabela_simbolos;
  // endereços de memoria
  vector<int> enderecos;
  // inicializar o vetor de endereços com 0
  enderecos.push_back(0);
  // imprimir o vetor de endereços
  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
    if (linha->size() >= 2 && linha->at(1) == ":") {
      if (linha->size() >= 3 && regex_match(linha->at(2), reConst)) {
        enderecos.push_back(enderecos.back() + 1);
        tabela_simbolos[linha->at(0)] = enderecos[enderecos.size() - 2];
      } else if (linha->size() >= 3 && regex_match(linha->at(2), reSpace)) {
        if (linha->size() == 4) {
          enderecos.push_back(enderecos.back() + stoi(linha->at(3)));  // TESTE : SPACE 3
          tabela_simbolos[linha->at(0)] = enderecos[enderecos.size() - 2];
        } else {
          enderecos.push_back(enderecos.back() + 1);
          tabela_simbolos[linha->at(0)] = enderecos[enderecos.size() - 2];
        }
      } else {
        tabela_simbolos[linha->at(0)] = enderecos.back();
      }
    } else {
      if (opcode.find(linha->at(0)) != opcode.end()) {
        if (regex_match(linha->at(0), reCopy)) {
          enderecos.push_back(enderecos.back() + 3);
        } else if (regex_match(linha->at(0), reStop)) {
          enderecos.push_back(enderecos.back() + 1);
        } else {
          enderecos.push_back(enderecos.back() + 2);
        }
      }
    }
  }
  // gerar o codigo objeto
  // percorrer o map
  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
    // percorrer o vetor de tokens
    // verificar se o token é um opcode
    if (linha->size() > 2 && linha->at(1) == ":") {
      if (regex_match(linha->at(2), reConst)) {
        // se for em hexadecimal transformar em decimal
        regex reHex("-?0x[0-9A-Fa-f]+");
        if (regex_match(linha->at(3), reHex)) {
          objetos.push_back(to_string(stoi(linha->at(3), nullptr, 16)));
        } else {
          objetos.push_back(linha->at(3));
        }
      } else if (regex_match(linha->at(2), reSpace)) {
        if (linha->size() == 4) {
          for (int i = 0; i < stoi(linha->at(3)); i++) {
            objetos.push_back("0");
          }
        } else {
          objetos.push_back("0");
        }
      }
      continue;
    }

    if (opcode.find(linha->at(0)) != opcode.end()) {
      // se for, adicionar o opcode ao vetor de objetos
      objetos.push_back(to_string(opcode[linha->at(0)]));
      // verificar se o proximo token está na tabela de simbolos
      // Stop não tem operandos
      if (regex_match(linha->at(0), reStop)) {
        continue;
      }
      // SPACE não tem operandos e não é um opcode colocar 0
      // Const tem 1 operando e não é um opcode colocar operando
      // Copy tem 2 operandos
      if (regex_match(linha->at(0), reCopy)) {
        // verificar se o proximo token está na tabela de simbolos
        if (tabela_simbolos.find(linha->at(1)) != tabela_simbolos.end()) {
          // se estiver, adicionar o endereço de memoria do token ao vetor de objetos
          objetos.push_back(to_string(tabela_simbolos[linha->at(1)]));
        }
        // verificar se o proximo token está na tabela de simbolos
        if (tabela_simbolos.find(linha->at(3)) != tabela_simbolos.end()) {
          // se estiver, adicionar o endereço de memoria do token ao vetor de objetos
          objetos.push_back(to_string(tabela_simbolos[linha->at(3)]));
        }
        continue;
      }
      if (tabela_simbolos.find(linha->at(1)) != tabela_simbolos.end()) {
        // se estiver, adicionar o endereço de memoria do token ao vetor de objetos
        objetos.push_back(to_string(tabela_simbolos[linha->at(1)]));
      } else {
        // se não estiver, adicionar o token ao vetor de objetos
        objetos.push_back(linha->at(1));
      }
    }
  }
  return objetos;
}

map<string, int> obterOpcodesNumericos() {
  // Declare a map to store the instructions
  map<string, int> opcode;

  // Populate the map with some instructions
  opcode["ADD"] = 1;
  opcode["SUB"] = 2;
  opcode["MULT"] = 3;
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
  // versoes minusculas
  opcode["add"] = 1;
  opcode["sub"] = 2;
  opcode["mult"] = 3;
  opcode["mul"] = 3;
  opcode["div"] = 4;
  opcode["jmp"] = 5;
  opcode["jmpn"] = 6;
  opcode["jmpp"] = 7;
  opcode["jmpz"] = 8;
  opcode["copy"] = 9;
  opcode["load"] = 10;
  opcode["store"] = 11;
  opcode["input"] = 12;
  opcode["output"] = 13;
  opcode["stop"] = 14;
  // Return the map
  return opcode;
}
