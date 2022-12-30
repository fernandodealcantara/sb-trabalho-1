#include "pre.h"

LinhaMap processarLinhas(string conteudo) {
  // Imprima o conteúdo linha a linha
  istringstream stream(conteudo);
  // mostrar o conteudo do arquivo
  string linha;
  LinhaMap linhas;
  while (getline(stream, linha)) {
    linhas.insert(pair<int, vector<string>>(linhas.size() + 1, tokenize(linha)));
  }

  // remover vetores vazios
  for (auto it = linhas.begin(); it != linhas.end();) {
    if (it->second.empty()) {
      it = linhas.erase(it);
    } else {
      ++it;
    }
  }
  return linhas;
}

// tokens validos
vector<string> tokenize(const string& str) {
  vector<string> tokens;
  string token;

  int i = 0;
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

LinhaMap processarEquates(const LinhaMap& linhas) {
  LinhaMap linhasProcessadas;
  map<string, string> equMap;
  vector<int> linhasIgnorar;

  for (auto linha = linhas.begin(); linha != linhas.end(); ++linha) {
    // percerrer até encontrar SECTION
    if (regex_match(linha->second[0], reSection) && regex_match(linha->second[1], reText)) {
      break;
    }
    // se for EQU, adicionar ao map linha.second[0] = label, linha.second[3] = value,
    // considerar label: enter EQU value

    vector<string> tokens = linha->second;
    if (tokens.size() == 4 && regex_match(tokens[0], reLabel) && tokens[1] == ":" &&
        regex_match(tokens[2], reEqu) && regex_match(tokens[3], reValue) &&
        equMap.find(tokens[0]) == equMap.end()) {
      equMap[tokens[0]] = tokens[3];
      linhasIgnorar.push_back(linha->first);
    }
    if (tokens.size() == 2 && regex_match(tokens[0], reLabel) && tokens[1] == ":") {
      string label = tokens[0];
      int linhaLabel = linha->first;
      ++linha;
      if (linha == linhas.end()) {
        break;
      }

      tokens = linha->second;
      if (tokens.size() == 2 && regex_match(tokens[0], reEqu) && regex_match(tokens[1], reValue)) {
        equMap[label] = tokens[1];
        linhasIgnorar.push_back(linhaLabel);
        linhasIgnorar.push_back(linha->first);
      }
    }
  }

  // substituir EQU por valor e ignorar linhas EQU
  for (const auto& linha : linhas) {
    if (find(linhasIgnorar.begin(), linhasIgnorar.end(), linha.first) != linhasIgnorar.end()) {
      continue;
    }
    vector<string> tokens = linha.second;
    for (int i = 0; i < tokens.size(); i++) {
      if (equMap.find(tokens[i]) != equMap.end()) {
        tokens[i] = equMap[tokens[i]];
      }
    }
    linhasProcessadas[linha.first] = tokens;
  }

  return linhasProcessadas;
}

LinhaMap processarIfs(const LinhaMap& linhas) {
  // se for IF, verificar se o valor é 0, se for, ignorar a linha seguinte, do contrario, ignorar a
  // linha do IF
  LinhaMap linhasProcessadas;
  vector<int> linhasIgnorar;

  for (const auto& linha : linhas) {
    vector<string> tokens = linha.second;
    if (regex_match(tokens[0], reIf)) {
      linhasIgnorar.push_back(linha.first);
      if (regex_match(tokens[1], reValue) && stoi(tokens[1]) == 0) {
        linhasIgnorar.push_back(linha.first + 1);
      }
    }
  }

  for (const auto& linha : linhas) {
    if (find(linhasIgnorar.begin(), linhasIgnorar.end(), linha.first) != linhasIgnorar.end()) {
      continue;
    }
    linhasProcessadas[linha.first] = linha.second;
  }

  return linhasProcessadas;
}