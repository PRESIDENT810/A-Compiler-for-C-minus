# A-Compiler-for-C-minus
Course project from CSC4180

The grammar we use:

program: varDecls stmts

varDecls: varDecl varDecls

varDecls: nullStr

varDecl: INT declList SEMI

declList: decl declListPostfix

declListPostfix: COMMA declList

declListPostfix: nullStr

decl: ID declPostfix

declPostfix: ASSIGN INTNUM

declPostfix: LSQUARE INTNUM RSQUARE

declPostfix: nullStr

codeBlock: stmt

codeBlock: LBRACE stmts RBRACE

stmts: stmt stmtsPostfix

stmtsPostfix: stmts

stmtsPostfix: nullStr

stmt: assignStmt SEMI

stmt: controlStmt

stmt: rwStmt SEMI

stmt: SEMI

controlStmt: ifStmts

controlStmt: whileStmt

controlStmt: dowhileStmt SEMI

controlStmt: returnStmt SEMI

rwStmt: rStmt

rwStmt: wStmt

assignStmt: ID assignStmtPostfix

assignStmtPostfix: LSQUARE exp RSQUARE ASSIGN exp

assignStmtPostfix: ASSIGN exp

ifStmts: ifStmt ifStmtsPostfix

ifStmtsPostfix: nullStr

ifStmtsPostfix: ELSE codeBlock

ifStmt: IF LPAR exp RPAR codeBlock

whileStmt: WHILE LPAR exp RPAR codeBlock

dowhileStmt: DO codeBlock WHILE LPAR exp RPAR

returnStmt: RETURN

rStmt: READ LPAR ID RPAR

wStmt: WRITE LPAR exp RPAR

exp: realExp OpExp

realExp: ID realExpPostfix

realExpPostfix: nullStr

realExpPostfix: LSQUARE exp RSQUARE

realExp: INTNUM

realExp: NOT_OP exp

realExp: LPAR exp RPAR

realExp: MINUS exp

OpExp: nullStr

OpExp: AND_OP realExp OpExp

OpExp: OR_OP realExp OpExp

OpExp: PLUS realExp OpExp

OpExp: MINUS realExp OpExp

OpExp: MUL_OP realExp OpExp

OpExp: DIV_OP realExp OpExp

OpExp: LT realExp OpExp

OpExp: GT realExp OpExp

OpExp: EQ realExp OpExp

OpExp: NOTEQ realExp OpExp

OpExp: LTEQ realExp OpExp

OpExp: GTEQ realExp OpExp

OpExp: SHL_OP realExp OpExp

OpExp: SHR_OP realExp OpExp

OpExp: ANDAND realExp OpExp

OpExp: OROR realExp OpExp
