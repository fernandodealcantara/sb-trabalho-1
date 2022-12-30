#include "utilitarios.h"

using namespace std;

int verificarArgumentos(int argc, char* argv[]) {
  // argv[0] = ./montador
  // argv[1] = -<op>
  // argv[2] = <arquivo>
  // O arquivo de entrada é um arquivo .asm a extensão não será passada como
  // argumento Verifica se o número de argumentos é válido
  if (argc != 3) {
    throw runtime_error("Número de argumentos inválido");
  }
  // Verifica se o segundo argumento é válido
  if (argv[1][0] != '-') {
    throw runtime_error("Segundo argumento inválido");
  }
  if (argv[1][1] != 'p' && argv[1][1] != 'm' && argv[1][1] != 'o') {
    throw runtime_error("Segundo argumento inválido");
  }
  // Verifica se o terceiro argumento é válido, não deve conter extensão
  string arquivo = argv[2];
  if (arquivo.find('.') != string::npos) {
    throw runtime_error("Terceiro argumento inválido");
  }

  return 0;
}

string lerArquivo(string arquivo) {
  // Abra o arquivo em modo de leitura texto
  ifstream file(arquivo);
  if (!file.is_open()) {
    throw runtime_error("Error opening file");
  }

  // Crie um buffer de memória para armazenar o conteúdo do arquivo
  stringstream buffer;

  // Copie o conteúdo do arquivo para o buffer
  buffer << file.rdbuf();

  // Feche o arquivo
  file.close();

  // Obtenha o conteúdo do buffer como uma string
  string conteudo = buffer.str();

  return conteudo;
}

void salvarArquivo(string arquivo, const LinhaMap& linhas) {
  // Abra o arquivo em modo de escrita texto
  ofstream file(arquivo);
  if (!file.is_open()) {
    throw runtime_error("Error opening file");
  }

  // Escreva o conteúdo do arquivo
  for (auto it = linhas.begin(); it != linhas.end(); ++it) {
    for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      file << *it2 << " ";
    }
    file << endl;
  }

  // Feche o arquivo
  file.close();
}

void dumpMap(const LinhaMap& linhas) {
  for (auto it = linhas.begin(); it != linhas.end(); ++it) {
    cout << it->first << " => ";
    for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      cout << *it2 << " ";
    }
    cout << endl;
  }
}

void dumpMnt(const MNTMap& mnt) {
  for (auto it = mnt.begin(); it != mnt.end(); ++it) {
    cout << it->first << " => N args: " << it->second.numArgs << " Linha:" << it->second.linha << endl;
  }
}