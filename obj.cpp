#include "obj.h"

void gerarCodigoObjeto(const Codigo &codigo)
{
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
  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha)
  {
    if (linha->size() >= 2 && linha->at(1) == ":")
    {
      if (linha->size() >= 3 && regex_match(linha->at(2), reConst))
      {
        enderecos.push_back(enderecos.back() + 1);
        tabela_simbolos[linha->at(0)] = enderecos[enderecos.size() - 2];
      }
      else if (linha->size() >= 3 && regex_match(linha->at(2), reSpace))
      {
        if (linha->size() == 4)
        {
          enderecos.push_back(enderecos.back() + stoi(linha->at(3))); // TESTE : SPACE 3
          tabela_simbolos[linha->at(0)] = enderecos[enderecos.size() - 2];
        }
        else
        {
          enderecos.push_back(enderecos.back() + 1);
          tabela_simbolos[linha->at(0)] = enderecos[enderecos.size() - 2];
        }
      }
      else
      {
        tabela_simbolos[linha->at(0)] = enderecos.back();
      }
    }
    else
    {
      if (opcode.find(linha->at(0)) != opcode.end())
      {
        if (regex_match(linha->at(0), reCopy))
        {
          enderecos.push_back(enderecos.back() + 3);
        }
        else if (regex_match(linha->at(0), reStop))
        {
          enderecos.push_back(enderecos.back() + 1);
        }
        else
        {
          enderecos.push_back(enderecos.back() + 2);
        }
      }
    }
  }
  // imprimir o vetor de endereços
  for (auto it = enderecos.begin(); it != enderecos.end(); ++it)
  {
    cout << *it << " ";
  }
  cout << endl;
  // imprimir a tabela de simbolos
  for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); ++it)
  {
    cout << it->first << " " << it->second << endl;
  }

  // gerar o codigo objeto
  // percorrer o map
  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha)
  {
    // percorrer o vetor de tokens
    // verificar se o token é um opcode
    if (linha->size() > 2 && linha->at(1) == ":")
    {
      if (regex_match(linha->at(2), reConst))
      {

        objetos.push_back(linha->at(3));
      }
      else if (regex_match(linha->at(2), reSpace))
      {

        objetos.push_back("0");
      }
      continue;
    }

    if (opcode.find(linha->at(0)) != opcode.end())
    {
      // se for, adicionar o opcode ao vetor de objetos
      objetos.push_back(to_string(opcode[linha->at(0)]));
      // verificar se o proximo token está na tabela de simbolos
      // Stop não tem operandos
      if (regex_match(linha->at(0), reStop))
      {
        continue;
      }
      // SPACE não tem operandos e não é um opcode colocar 0
      // Const tem 1 operando e não é um opcode colocar operando
      // Copy tem 2 operandos
      if (regex_match(linha->at(0), reCopy))
      {
        // verificar se o proximo token está na tabela de simbolos
        if (tabela_simbolos.find(linha->at(1)) != tabela_simbolos.end())
        {
          // se estiver, adicionar o endereço de memoria do token ao vetor de objetos
          objetos.push_back(to_string(tabela_simbolos[linha->at(1)]));
        }
        // verificar se o proximo token está na tabela de simbolos
        if (tabela_simbolos.find(linha->at(3)) != tabela_simbolos.end())
        {
          // se estiver, adicionar o endereço de memoria do token ao vetor de objetos
          objetos.push_back(to_string(tabela_simbolos[linha->at(3)]));
        }
        continue;
      }
      if (tabela_simbolos.find(linha->at(1)) != tabela_simbolos.end())
      {
        // se estiver, adicionar o endereço de memoria do token ao vetor de objetos
        objetos.push_back(to_string(tabela_simbolos[linha->at(1)]));
      }
      else
      {
        // se não estiver, adicionar o token ao vetor de objetos
        objetos.push_back(linha->at(1));
      }
    }
  }
  // imprimir o vetor de objetos
  for (auto it = objetos.begin(); it != objetos.end(); ++it)
  {
    cout << *it << endl;
  }
}
/*
void gerarCodigoObjeto(const LinhaMap &linhas) {
  map<string, int> opcode = obterOpcodesNumericos();
  // vetor de dos objetos
  vector<string> objetos;
  // map tabela de simbolos
  map<string, int> tabela_simbolos;
  // endereços de memoria
  vector<int> enderecos;
  // inicializar o vetor de endereços com 0
  enderecos.push_back(0);
  for (auto it = linhas.begin(); it != linhas.end(); ++it) {
    // se o token for uma instrução, soma 2 ao endereço de memoria anterior e guarda-la no vetor
de
    // endereços Copy é uma instrução que tem 2 operandos, então soma 3 ao endereço de memoria
    // anterior e guarda-la no vetor de endereços Stop é uma instrução que não tem operandos,
então
    // soma 1 ao endereço de memoria anterior e guarda-la no vetor de endereços Não considerar os
    // labels
    if (it->second[1] == ":") {
      // Veridicar se é um instrução ou diretiva
      if (opcode.find(it->second[2]) != opcode.end() || regex_match(it->second[2], reConst) ||
          regex_match(it->second[2], reSpace)) {
        if (regex_match(it->second[2], reCopy)) {
          enderecos.push_back(enderecos.back() + 3);
        } else if (regex_match(it->second[2], reStop)) {
          enderecos.push_back(enderecos.back() + 1);
        } else if (regex_match(it->second[2], reConst)) {
          enderecos.push_back(enderecos.back() + 1);
        } else if (regex_match(it->second[2], reSpace)) {
          if (it->second.size() == 3) {
            enderecos.push_back(enderecos.back() + 1);
          } else {
            enderecos.push_back(enderecos.back() + stoi(it->second[3]));
          }
        } else {
          enderecos.push_back(enderecos.back() + 2);
        }
      }
    } else {
      if (opcode.find(it->second[0]) != opcode.end()) {
        if (regex_match(it->second[2], reCopy)) {
          enderecos.push_back(enderecos.back() + 3);
        } else if (regex_match(it->second[2], reStop)) {
          enderecos.push_back(enderecos.back() + 1);
        } else {
          enderecos.push_back(enderecos.back() + 2);
        }
      }
    }

    // percorrer o vetor de tokens e verificar se é label, para ser label o proximo token tem que
    // ser :
    const vector<string> &tokens = it->second;
    for (int i = 0; i < tokens.size(); i++) {
      // somar 2 ao endereço de memoria anterior e guarda-la no vetor de endereços
      if (tokens[i] == ":") {
        // adicionar o token anterior na tabela de simbolos e seu endereço
        tabela_simbolos[tokens[i - 1]] = enderecos[enderecos.size() - 2];
      }
    }
  }
  // // imprimir o vetor de endereços
  // for (const int &endereco : enderecos)
  // {
  //   cout << endereco << " ";
  // }
  // cout << endl;
  // imprimir o mapa da tabela de simbolos
  for (auto it = tabela_simbolos.begin(); it != tabela_simbolos.end(); ++it) {
    cout << it->first << " " << it->second << endl;
  }

  // percorrer o map
  for (auto it = linhas.begin(); it != linhas.end(); ++it) {
    // percorrer o vetor de tokens
    for (const string &token : it->second) {
      // verificar se o token é um opcode
      if (opcode.find(token) != opcode.end()) {
        // se for, adicionar o opcode ao vetor de objetos
        objetos.push_back(to_string(opcode[token]));
        // verificar se o proximo token está na tabela de simbolos
      }
    }
  }

  // imprimir o vetor de objetos
  for (const string &objeto : objetos) {
    cout << objeto << " ";
  }
  cout << endl;
}
*/
map<string, int> obterOpcodesNumericos()
{
  // Declare a map to store the instructions
  map<string, int> opcode;

  // Populate the map with some instructions
  opcode["ADD"] = 1;
  opcode["SUB"] = 2;
  opcode["MULT"] = 3;
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
