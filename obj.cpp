#include "obj.h"

vector<string> gerarCodigoObjeto(const Codigo &codigo)
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
        // se for em hexadecimal transformar em decimal
        regex reHex("-?0x[0-9A-Fa-f]+");
        if (regex_match(linha->at(3), reHex))
        {
          objetos.push_back(to_string(stoi(linha->at(3), nullptr, 16)));
        }
        else
        {
          objetos.push_back(linha->at(3));
        }
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
  return objetos;
}

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
