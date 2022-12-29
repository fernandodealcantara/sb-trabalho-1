#include "pre.h"

map<int, vector<string>> processarLinhas(string conteudo) {
  // Imprima o conteúdo linha a linha
  istringstream stream(conteudo);
  // mostrar o conteudo do arquivo
  string linha;
  map<int, vector<string>> linhas;
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

map<int, vector<string>> processarEquates(const map<int, vector<string>>& linhas) {
  map<int, vector<string>> linhasProcessadas;
  map<string, string> equMap;
  vector<int> linhasEqu;

  regex label("[A-Za-z][A-Za-z0-9_]*");
  regex equ("EQU|equ");
  regex value("[0-9]+");

  for (const auto& linha : linhas) {
    // percerrer até encontrar SECTION
    if (linha.second[0] == "SECTION") {
      break;
    }
    // se for EQU, adicionar ao map linha.second[0] = label, linha.second[3] = value,
    vector<string> tokens = linha.second;
    if (regex_match(tokens[0], label) && tokens[1] == ":" && regex_match(tokens[2], equ) &&
        regex_match(tokens[3], value)) {
      equMap[tokens[0]] = tokens[3];
      linhasEqu.push_back(linha.first);
    }
  }

  // substituir EQU por valor e ignorar linhas EQU
  for (const auto& linha : linhas) {
    if (find(linhasEqu.begin(), linhasEqu.end(), linha.first) != linhasEqu.end()) {
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

map<int, vector<string>> processarIfs(const map<int, vector<string>>& linhas) {
  // se for IF, verificar se o valor é 0, se for, ignorar a linha seguinte, do contrario, ignorar a
  // linha do IF

  map<int, vector<string>> linhasProcessadas;
  vector<int> linhasIgnorar;

  regex ifRegex("IF|if");

  for (const auto& linha : linhas) {
    vector<string> tokens = linha.second;
    if (regex_match(tokens[0], ifRegex)) {
      linhasIgnorar.push_back(linha.first);
      if (stoi(tokens[1]) == 0) {
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