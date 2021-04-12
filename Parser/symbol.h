#ifndef PROJECT3_SYMBOL_H_
#define PROJECT3_SYMBOL_H_

#include <cstring>

const int nonTerminalCnt = 27;
const int terminalCnt = 40;

//
// the enumeration class for terminal & non-terminal symbols
//
enum class Symbol{
    // non-terminal symbols
    program = 0,
    varDecls = 1,
    varDecl = 2,
    declList = 3,
    declListPostfix = 4,
    decl = 5,
    declPostfix = 6,
    codeBlock = 7,
    stmts = 8,
    stmtsPostfix = 9,
    stmt = 10,
    controlStmt = 11,
    rwStmt = 12,
    assignStmt = 13,
    assignStmtPostfix = 14,
    ifStmts = 15,
    ifStmtsPostfix = 16,
    ifStmt = 17,
    whileStmt = 18,
    dowhileStmt = 19,
    returnStmt = 20,
    rStmt = 21,
    wStmt = 22,
    exp = 23,
    realExp = 24,
    realExpPostfix = 25,
    OpExp = 26,
    // reversed words
    INT = 27,
    MAIN = 28,
    VOID = 29,
    BREAK = 30,
    DO = 31,
    ELSE = 32,
    IF = 33,
    WHILE = 34,
    RETURN = 35,
    READ = 36,
    WRITE = 37,
    // special symbols
    LBRACE  =38,
    RBRACE = 39,
    LSQUARE = 40,
    RSQUARE = 41,
    LPAR = 42,
    RPAR = 43,
    SEMI = 44,
    PLUS = 45,
    MINUS = 46,
    MUL_OP = 47,
    DIV_OP = 48,
    AND_OP = 49,
    OR_OP = 50,
    NOT_OP = 51,
    ASSIGN = 52,
    LT = 53,
    GT = 54,
    SHL_OP = 55,
    SHR_OP = 56,
    EQ = 57,
    NOTEQ = 58,
    LTEQ = 59,
    GTEQ = 60,
    ANDAND = 61,
    OROR = 62,
    COMMA = 63,
    // regex symbols
    INTNUM = 64,
    ID = 65,
    // null string
    nullStr = 66,
};

#endif //PROJECT3_SYMBOL_H