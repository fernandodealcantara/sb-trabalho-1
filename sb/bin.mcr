SECTION TEXT 
INPUT OLD_DATA 
LOAD OLD_DATA 
L1 : DIV 1000 
STORE NEW_DATA 
MULT 1000 
STORE TMP_DATA 
TEST : 
INPUT DOIS 
LOAD DOIS 
LOAD OLD_DATA 
SUB TMP_DATA 
INPUT 1 
LOAD DOIS 
STORE TMP_DATA 
OUTPUT TMP_DATA 
LOAD OLD_DATA 
JMPP L1 
MULT DOIS 
INPUT 1 
LOAD DOIS 
DIV 1 
STOP 
SECTION DATA 
DOIS : CONST 2 
OLD_DATA : SPACE 
NEW_DATA : SPACE 
TMP_DATA : SPACE 