; Calcular os n primeiros termos da sequência de Fibonacci
mostrar_termos_1_1: equ 0 ; se for 1, mostrar os termos 1 e 1 se for 0, não mostrar
V_1: equ 0x1 ; valor 1 para constante
calcular_n_termos_fibonacci: MACRO &a,&b,&c ; calcular o n-ésimo termo da sequência de Fibonacci
load &a       ; &a
add &b        ; &a + &b
store &c      ; &c = &a + &b 
load &b       ; &b
store &a      ; &a = &b
load &c       ; &c
store &b      ; &b = &c
ENDMACRO

decrementar_qtd_termos_fibonacci: MACRO ; decrementar a quantidade de termos da sequência de Fibonacci
load qtd_termos_fibonacci
sub ONE
store qtd_termos_fibonacci
ENDMACRO

SECTION TEXT
INPUT qtd_termos_fibonacci ; Quantidade de termos da sequência de Fibonacci
copy ONE,arr      ; arr[0] = 1
copy ONE,arr+1    ; arr[1] = 1
IF mostrar_termos_1_1
OUTPUT arr        ; imprimir o primeiro termo da sequência de Fibonacci
IF mostrar_termos_1_1
OUTPUT arr+1      ; imprimir o segundo termo da sequência de Fibonacci
load qtd_termos_fibonacci
jmpn parar        ; se for negativo, não calcular
jmpz parar        ; se for zero, não calcular
fibonacci:        ; calcular os n primeiros termos da sequência de Fibonacci
calcular_n_termos_fibonacci arr,arr+1,arr+2
OUTPUT arr+2      ; imprimir o n-ésimo termo da sequência de Fibonacci
decrementar_qtd_termos_fibonacci
jmpp fibonacci    ; se for positivo, calcular o próximo termo da sequência de Fibonacci
parar:STOP

SECTION DATA
arr: SPACE 3
ONE: CONST V_1 ; constante 1 para decrementar a qtd de termos da seq. de Fibonacci
qtd_termos_fibonacci: SPACE