; Programa que calcula o valor de 5 + 7
; Deve gerar obj -> 10 9 1 10 11 11 13 11 14 5 7 0
N_CINCO: EQU 5
N_SETE: EQU 7
TRUE: EQU 1
FALSE: EQU 0
M1: MACRO &A,&B ; soma os valores de &A e &B
LOAD &A
ADD &B
ENDMACRO
SECTION TEXT
M2: MACRO ; soma os valores de V1 e V2 e guarda o resultado em RESULT
M1 V1,V2
STORE RESULT
ENDMACRO
IF FALSE
SUB V2
M2 ; invoca a macro M2
IF TRUE
OUTPUT RESULT ; imprime o resultado
STOP
SECTION DATA
V1: CONST N_CINCO
V2: CONST N_SETE
RESULT: SPACE