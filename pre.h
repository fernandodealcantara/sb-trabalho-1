#ifndef PRE_H
#define PRE_H

#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

map<int, vector<string>> processarEquates(const map<int, vector<string>>& linhas);
map<int, vector<string>> processarIfs(const map<int, vector<string>>& linhas);
map<int, vector<string>> processarLinhas(string conteudo);
vector<string> tokenize(const string& str);

#endif  // PRE_H