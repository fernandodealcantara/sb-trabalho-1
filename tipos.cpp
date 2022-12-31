#include "tipos.h"

// using regex
// tokens validos são: ADD, SUB, MUL, DIV, JMP, JMPN, JMPP, JMPZ, COPY, LOAD,
// STORE, INPUT, OUTPUT, STOP, SPACE, CONST, SECTION TEXT, SECTION DATA,
// PUBLIC, EXTERN, EQU, IF, MACRO, ENDMACRO, BEGIN, END, ":" ",",
// X+2(caractere+digito) e qualquer palavra que não seja uma instrução ou
// diretiva

// regex para tokens validos
string uppercaseInsts1Arg = "ADD|SUB|MUL|MULT|DIV|JMP|JMPN|JMPP|JMPZ|LOAD|STORE|INPUT|OUTPUT";
string lowercaseInsts1Arg = "add|sub|mul|mult|div|jmp|jmpn|jmpp|jmpz|load|store|input|output";
string uppercaseInsts = "ADD|SUB|MUL|MULT|DIV|JMP|JMPN|JMPP|JMPZ|COPY|LOAD|STORE|INPUT|OUTPUT|STOP";
string lowercaseInsts = "add|sub|mul|mult|div|jmp|jmpn|jmpp|jmpz|copy|load|store|input|output|stop";
string uppercaseDirectives = "SPACE|CONST|SECTION|PUBLIC|EXTERN|EQU|IF|MACRO|ENDMACRO|BEGIN|END";
string lowercaseDirectives = "space|const|section|public|extern|equ|if|macro|endmacro|begin|end";
string uppercaseSections = "TEXT|DATA";
string lowercaseSections = "text|data";
string labels = "[A-Za-z][A-Za-z0-9_]*[^_]?";
string values = "[0-9]+|[A-Za-z]+\\+[0-9]+|&[A-Za-z][A-Za-z0-9_]*[^_]?";
string separators = ":|,";

// instrucoes
regex reAdd("ADD|add");
regex reSub("SUB|sub");
regex reMul("MUL|mul|MULT|mult");
regex reDiv("DIV|div");
regex reJmp("JMP|jmp");
regex reJmpn("JMPN|jmpn");
regex reJmpp("JMPP|jmpp");
regex reJmpz("JMPZ|jmpz");
regex reCopy("COPY|copy");
regex reLoad("LOAD|load");
regex reStore("STORE|store");
regex reInput("INPUT|input");
regex reOutput("OUTPUT|output");
regex reStop("STOP|stop");
// diretivas
regex reSpace("SPACE|space");
regex reConst("CONST|const");
regex reSection("SECTION|section");
regex rePublic("PUBLIC|public");
regex reExtern("EXTERN|extern");
regex reEqu("EQU|equ");
regex reIf("IF|if");
regex reMacro("MACRO|macro");
regex reEndmacro("ENDMACRO|endmacro");
regex reBegin("BEGIN|begin");
regex reEnd("END|end");
// secoes
regex reText("TEXT|text");
regex reData("DATA|data");
// tokens validos
regex reLabel("[A-Za-z][A-Za-z0-9_]*[^_]?");
regex reValue("[0-9]+");
regex reSymbolPlusValue("[A-Za-z]+\\+[0-9]+");
regex reSeparator(":|,");
regex reComment(";.*");
regex reMacroParam("&[A-Za-z][A-Za-z0-9_]*[^_]?");
regex reParametro("#[0-9]+");
