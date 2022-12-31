#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <vector>

#include "analises.h"
#include "mcr.h"
#include "obj.h"
#include "pre.h"
#include "tipos.h"
#include "utilitarios.h"
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

int main(int argc, char *argv[])
{
  auto instructions = obterOpcodesNumericos();
  verificarArgumentos(argc, argv);
  char op = argv[1][1];
  string nome_arquivo = argv[2];
  string conteudo = lerArquivo(nome_arquivo + ".asm");
  vector<string> codigoObjeto;
  Codigo codigo = processarLinhas(conteudo);

  codigo = processarEquates(codigo);
  codigo = processarIfs(codigo);

  if (op == 'p')
  {
    salvarArquivo(nome_arquivo + ".pre", codigo);
    return 0;
  }

  codigo = processarMacros(codigo);

  if (op == 'm')
  {
    salvarArquivo(nome_arquivo + ".mcr", codigo);
    return 0;
  }

  for (int i = 0; i < codigo.size(); i++)
  {
    // analiseLexica(i + 1, codigo[i]);
    // analiseSintatica(i + 1, codigo[i]);
  }
  codigoObjeto = gerarCodigoObjeto(codigo);
  if (op == 'o')
  {
    //Salvar arquivo
      ofstream file(nome_arquivo + ".obj");
      if (!file.is_open())
      {
        throw runtime_error("Error opening file");
      }
      for (int i = 0; i < codigoObjeto.size(); i++)
      {
        file << codigoObjeto[i] << " ";
      }
      file.close();
    return 0;
  }
  // vector<string> tokens = obterTokens(codigo);
  // dumpLinhaCodigo(tokens);

  // dumpCodigo(codigo);

  return 0;
}
