#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <vector>

#include "src/analises.h"
#include "src/mcr.h"
#include "src/obj.h"
#include "src/pre.h"
#include "src/tipos.h"
#include "src/utilitarios.h"
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

int main(int argc, char* argv[]) {
  verificarArgumentos(argc, argv);
  char op = argv[1][1];

  string nome_arquivo = argv[2];
  string conteudo = lerArquivo(nome_arquivo + ".asm");

  Codigo codigo = processarLinhas(conteudo);

#ifdef DEBUG
  cout << "\nCodigo original:" << endl;
  dumpCodigo(codigo);
#endif

  codigo = processarEquates(codigo);
#ifdef DEBUG
  cout << "\nCodigo depois de processar equates:" << endl;
  dumpCodigo(codigo);
#endif
  codigo = processarIfs(codigo);
#ifdef DEBUG
  cout << "\nCodigo depois de processar ifs:" << endl;
  dumpCodigo(codigo);
#endif

  if (op == 'p') {
    salvarArquivo(nome_arquivo + ".pre", codigo);
    return 0;
  }

  codigo = processarMacros(codigo);
#ifdef DEBUG
  cout << "\nCodigo depois de processar macros:" << endl;
  dumpCodigo(codigo);
#endif

  if (op == 'm') {
    salvarArquivo(nome_arquivo + ".mcr", codigo);
    return 0;
  }

  analisarCodigo(codigo);

  vector<string> codigoObjeto = processarObjeto(codigo);
#ifdef DEBUG
  cout << "\nCodigo objeto:" << endl;
  dumpLinhaCodigo(codigoObjeto);
#endif

  if (op == 'o') {
    salvarArquivoObjeto(nome_arquivo + ".obj", codigoObjeto);
    return 0;
  }

  return 0;
}
