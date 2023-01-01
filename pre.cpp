#include "pre.h"

// remove comentarios, espaços, quebra linhas e transforma no formato de lista
Codigo processarLinhas(string conteudo) {
  // Imprima o conteúdo linha a linha
  istringstream stream(conteudo);
  // mostrar o conteudo do arquivo
  string linha;
  Codigo codigo;
  while (getline(stream, linha)) {
    // se não for vazio, adicionar ao codigo
    LinhaCodigo linha_codigo = tokenize(linha);
    // quebrar label e instrucaao em linhas diferentes
    while (linha_codigo.size() > 2 && linha_codigo[1] == ":" &&
           !regex_match(linha_codigo[2], reSpace) && !regex_match(linha_codigo[2], reConst)) {
      LinhaCodigo linha_label;
      linha_label.push_back(linha_codigo[0]);
      linha_label.push_back(linha_codigo[1]);
      codigo.push_back(linha_label);
      linha_codigo.erase(linha_codigo.begin(), linha_codigo.begin() + 2);
    }

    if (!linha_codigo.empty()) {
      codigo.push_back(linha_codigo);
    }
  }

  return codigo;
}

// quebrar linha em tokens
LinhaCodigo tokenize(const string& str) {
  LinhaCodigo tokens;
  string token;

  for (char c : str) {
    if (c == ';') {
      break;
    }
    if (c == ' ' || c == ',' || c == ':') {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
      if (c != ' ') {
        tokens.push_back(string(1, c));
      }
    } else {
      token += c;
    }
  }

  if (!token.empty()) {
    tokens.push_back(token);
  }

  return tokens;
}

Codigo processarEquates(const Codigo& codigo) {
  Codigo codigoProcessado;
  map<string, string> equMap;
  vector<int> linhasIgnorar;

  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
    LinhaCodigo linha_codigo = *linha;
    // percerrer até encontrar SECTION
    if (linha_codigo.size() >= 2 && regex_match(linha_codigo[0], reSection) &&
        regex_match(linha_codigo[1], reText)) {
      break;
    }
    if (linha_codigo.size() == 2 && regex_match(linha_codigo[0], reLabel) &&
        linha_codigo[1] == ":") {
      string label = linha_codigo[0];
      int linhaLabel = linha - codigo.begin();
      ++linha;
      if (linha == codigo.end()) {
        break;
      }

      linha_codigo = *linha;
      if (linha_codigo.size() == 2 && regex_match(linha_codigo[0], reEqu) &&
          regex_match(linha_codigo[1], reValue)) {
        equMap[label] = linha_codigo[1];
        linhasIgnorar.push_back(linhaLabel);
        linhasIgnorar.push_back(linha - codigo.begin());
      }
    }
  }

  for (int i = 0; i < codigo.size(); ++i) {
    if (find(linhasIgnorar.begin(), linhasIgnorar.end(), i) != linhasIgnorar.end()) {
      continue;
    }
    LinhaCodigo linha_codigo = codigo[i];
    for (int j = 0; j < linha_codigo.size(); ++j) {
      if (equMap.find(linha_codigo[j]) != equMap.end()) {
        linha_codigo[j] = equMap[linha_codigo[j]];
      }
    }
    codigoProcessado.push_back(linha_codigo);
  }

  return codigoProcessado;
}

Codigo processarIfs(const Codigo& codigo) {
  Codigo codigoProcessado;
  vector<int> linhasIgnorar;

  for (auto linha = codigo.begin(); linha != codigo.end(); ++linha) {
    LinhaCodigo linha_codigo = *linha;

    if (linha_codigo.size() >= 2 && regex_match(linha_codigo[0], reIf)) {
      int linhaIf = linha - codigo.begin();
      linhasIgnorar.push_back(linhaIf);

      if (regex_match(linha_codigo[1], reValue) && stoi(linha_codigo[1]) == 0) {
        ++linha;
        if (linha == codigo.end()) {
          break;
        }
        linha_codigo = *linha;
        linhasIgnorar.push_back(linha - codigo.begin());
      }
    }
  }

  for (int i = 0; i < codigo.size(); ++i) {
    if (find(linhasIgnorar.begin(), linhasIgnorar.end(), i) != linhasIgnorar.end()) {
      continue;
    }
    codigoProcessado.push_back(codigo[i]);
  }

  return codigoProcessado;
}