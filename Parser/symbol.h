#include <cstring>

//
// the enumeration class for terminal & non-terminal symbols
//
enum class Symbol{
    // non-terminal symbols
    program = 0,
    varDecls = 1,
    varDecl = 2,
    declList = 3,
    decl = 4,
    codeBlock = 5,
    stmts = 6,
    stmt = 7,
    controlStmt = 8,
    rwStmt = 9,
    assignStmt = 10,
    ifStmts = 11,
    ifStmt = 12,
    whileStmt = 13,
    dowhileStmt = 14,
    returnStmt = 15,
    rStmt = 16,
    wStmt = 17,
    exp = 18,
    realExp = 19,
    OpExp = 20,
    // reversed words
    INT = 21,
    MAIN = 22,
    VOID = 23,
    BREAK = 24,
    DO = 25,
    ELSE = 26,
    IF = 27,
    WHILE = 28,
    RETURN = 29,
    READ = 30,
    WRITE = 31,
    // special symbols
    LBRACE  =32,
    RBRACE = 33,
    LSQUARE = 34,
    RSQUARE = 35,
    LPAR = 36,
    RPAR = 37,
    SEMI = 38,
    PLUS = 39,
    MINUS = 40,
    MUL_OP = 41,
    DIV_OP = 42,
    AND_OP = 43,
    OR_OP = 44,
    NOT_OP = 45,
    ASSIGN = 46,
    LT = 47,
    GT = 48,
    SHL_OP = 49,
    SHR_OP = 50,
    EQ = 51,
    NOTEQ = 52,
    LTEQ = 53,
    GTEQ = 54,
    ANDAND = 55,
    OROR = 56,
    COMMA = 57,
    // regex symbols
    INTNUM = 58,
    ID = 59,
    // null string
    nullStr = 60,
};

bool isTerminal(Symbol symbol){
    return static_cast<int>(symbol) > 20;
}