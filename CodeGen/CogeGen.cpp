#include <stdlib.h>
#include "CogeGen.h"
#include "VM.h"

VM* vm;

void programGen(TreeNode* crtNode){
    varDeclsGen(crtNode->myChildren[0]);
    stmtsGen(crtNode->myChildren[1]);
}

void varDeclsGen(TreeNode* crtNode){
    if (crtNode->myChildren[0]->mySymbol != Symbol::nullStr){
        varDeclGen(crtNode->myChildren[0]);
        varDeclsGen(crtNode->myChildren[1]);
    }
}

void varDeclGen(TreeNode* crtNode){
    declListGen(crtNode->myChildren[1]);
}

void declListGen(TreeNode* crtNode){
    declGen(crtNode->myChildren[0]);
    if (crtNode->myChildren.size() == 3){
        declListGen(crtNode->myChildren[2]);
    }
}

void declGen(TreeNode* crtNode){
    if (crtNode->myChildren.size() == 1){
        vm->registerVar(crtNode->myChildren[0]->semanticValue);
    } else if (crtNode->myChildren.size() == 3){
        Ptr* ptr = vm->registerVar(crtNode->myChildren[0]->semanticValue);
        // assign value to this address
        int value = atoi(crtNode->myChildren[2]->semanticValue);
        printf("li $v0 %d\n", value);
        printf("sw $v0 %d($sp)\n", ptr->getAddr());
    } else if (crtNode->myChildren.size() == 4){
        int len = atoi(crtNode->myChildren[2]->semanticValue);
        Ptr* ptr = vm->registerArr(crtNode->myChildren[0]->semanticValue, len);
    }
}

void codeBlockGen(TreeNode* crtNode){
    if (crtNode->myChildren.size() == 3){
        stmtsGen(crtNode->myChildren[1]);
    } else{
        stmtGen(crtNode->myChildren[0]);
    }
}

void stmtsGen(TreeNode* crtNode){
    stmtGen(crtNode->myChildren[0]);
    if (crtNode->myChildren.size() == 2){
        stmtsGen(crtNode->myChildren[1]);
    }
}

void stmtGen(TreeNode* crtNode){
    if (crtNode->myChildren[0]->mySymbol == Symbol::SEMI){
        return;
    } else if (crtNode->myChildren[0]->mySymbol == Symbol::assignStmt){
        assignStmtGen(crtNode->myChildren[0]);
    } else if (crtNode->myChildren[0]->mySymbol == Symbol::controlStmt){
        controlStmtGen(crtNode->myChildren[0]);
    } else if (crtNode->myChildren[0]->mySymbol == Symbol::rwStmt){
        rwStmtGen(crtNode->myChildren[0]);
    }

}

void controlStmtGen(TreeNode* crtNode){
    if (crtNode->myChildren[0]->mySymbol == Symbol::ifStmts){
        ifStmtsGen(crtNode->myChildren[0]);
    } else if (crtNode->myChildren[0]->mySymbol == Symbol::whileStmt){
        whileStmtGen(crtNode->myChildren[0]);
    } else if (crtNode->myChildren[0]->mySymbol == Symbol::dowhileStmt){
        dowhileStmtGen(crtNode->myChildren[0]);
    } else if (crtNode->myChildren[0]->mySymbol == Symbol::returnStmt){
        returnStmtGen(crtNode->myChildren[0]);
    }
}

void rwStmtGen(TreeNode* crtNode){
    if (crtNode->myChildren[0]->mySymbol == Symbol::rStmt){
        rStmtGen(crtNode->myChildren[0]);
    } else if (crtNode->myChildren[0]->mySymbol == Symbol::wStmt){
        wStmtGen(crtNode->myChildren[0]);
    }
}

//
// for assigning a single variable
// it first calculate the value of exp (store in $t3)
// and then save it into the memory address
// for assigning an array
// it first calculate the value of offset (store in $t3)
// then it save it into $a0, and add it to $sp as offset
// then it calculate the value of exp (store in $t3)
// then save it into the memory address
// at last subtract the offset from $sp
//
void assignStmtGen(TreeNode* crtNode){
    if (crtNode->myChildren.size() == 3){
        expGen(crtNode->myChildren[2]);
        // assign a variable
        Ptr* ptr = vm->getPtr(crtNode->myChildren[0]->semanticValue);
        printf("sw $t3 %d($sp)\n", ptr->getAddr());
    } else if (crtNode->myChildren.size() == 6){
        // assign an array
        expGen(crtNode->myChildren[2]);
        printf("add $a0 $zero $t3\n");
        printf("add $sp $a0 $zero\n");
        expGen(crtNode->myChildren[5]);
        Ptr* ptr = vm->getPtr(crtNode->myChildren[0]->semanticValue);
        printf("lw $t3 $%d($sp)\n", ptr->getAddr());
        printf("sub $sp $sp $a0\n");
    }
}

//
// exp will goto PC+8 when exp is true, and goto PC+4 when exp is false
// so after we compute exp, the next instruction should be jal to ELSE
// and the next instruction should be IF block,
// with its last instruction being jal to the end of ELSE block
// e.g.
// bne $1 $2 1
// jal LABEL 2
// jal LABEL 1
// Label 1
// IF block
// jal LABEL 3
// Label 2
// ELSE block
// jal LABEL 3
// Label 3
//
void ifStmtsGen(TreeNode* crtNode){
    // bne $1 $2 1
    expGen(crtNode->myChildren[0]->myChildren[2]);
    // jal LABEL 2
    // jal LABEL 1
    // Label 1
    int label1 = vm->newLabel();
    int label2 = vm->newLabel();
    int label3 = vm->newLabel();
    printf("jal LABEL%d\n", label2);
    printf("jal LABEL%d\n", label1);
    printf("LABEL%d\n", label1);
    // IF block
    codeBlockGen(crtNode->myChildren[4]);
    // jal LABEL 3
    // Label 2
    printf("jal LABEL%d\n", label3);
    printf("LABEL%d\n", label2);
    // ELSE block
    if (crtNode->myChildren.size() == 3){
        codeBlockGen(crtNode->myChildren[2]);
    }
    // jal LABEL 3
    // Label 3
    printf("jal LABEL%d\n", label3);
    printf("LABEL%d\n", label3);
}

void ifStmtGen(TreeNode* crtNode){

}

//
// LABEL1
// bne $1 $2 1
// jal LABEL2
// codeBlock
// jal LABEL1
// LABEL2
//
void whileStmtGen(TreeNode* crtNode){
    int label1 = vm->newLabel();
    int label2 = vm->newLabel();
    printf("LABEL%d", label1);
    expGen(crtNode->myChildren[2]);
    printf("jal LABEL%d\n", label2);
    codeBlockGen(crtNode->myChildren[4]);
    printf("jal LABEL%d\n", label1);
    printf("LABEL%d\n", label2);
}

//
// LABEL1
// codeBlock
// bne $1 $2 1
// jal LABEL2
// jal LABEL1
// LABEL2
//
void dowhileStmtGen(TreeNode* crtNode){
    int label1 = vm->newLabel();
    int label2 = vm->newLabel();
    printf("LABEL%d\n", label1);
    codeBlockGen(crtNode->myChildren[1]);
    expGen(crtNode->myChildren[4]);
    printf("jal LABEL%d\n", label2);
    printf("jal LABEL%d\n", label1);
    printf("LABEL%d\n", label2);
}

void returnStmtGen(TreeNode* crtNode){

}

//
// the result of scanf is saved at $t3 by default
//
void rStmtGen(TreeNode* crtNode){
    printf("jal scanf");
    Ptr* ptr = vm->getPtr(crtNode->myChildren[2]->semanticValue);
    printf("sw $t3 %d($sp)\n", ptr->getAddr());
}

//
// the result to be output is saved at $t3 by default
//
void wStmtGen(TreeNode* crtNode){
    expGen(crtNode->myChildren[2]);
    printf("jal printf");
}

//
// by default, the result of exp will be put on $t3
//
void expGen(TreeNode* crtNode){
    if (crtNode->myChildren.size() == 1 && crtNode->myChildren[0]->mySymbol == Symbol::ID){
        // exp: ID
        Ptr* ptr = vm->getPtr(crtNode->myChildren[0]->semanticValue);
        printf("lw $t3 %d($sp)\n", ptr->getAddr());
    } else if (crtNode->myChildren.size() == 1 && crtNode->myChildren[0]->mySymbol == Symbol::INTNUM){
        // exp: INTNUM
        int num = atoi(crtNode->myChildren[0]->semanticValue);
        printf("addi $t3 $zero %d\n", num);
    } else if (crtNode->myChildren.size() == 4){
        // exp: ID LSQUARE exp RSQUARE
        expGen(crtNode->myChildren[2]);
        printf("add $sp $t3 $zero\n");
        Ptr* ptr = vm->getPtr(crtNode->myChildren[0]->semanticValue);
        printf("lw $t3 $%d($sp)\n", ptr->getAddr());
        printf("sub $sp $sp $t3\n");
    } else if (crtNode->myChildren.size() == 2 && crtNode->myChildren[0]->mySymbol == Symbol::NOT_OP){
        // exp: NOT_OP exp
        expGen(crtNode->myChildren[1]);
        printf("nor $t3, $t3, $t3\n");
    } else if (crtNode->myChildren.size() == 2 && crtNode->myChildren[0]->mySymbol == Symbol::MINUS){
        // exp: MINUS exp
        expGen(crtNode->myChildren[1]);
        printf("sub $t3 $zero $t3\n");
    } else if (crtNode->myChildren.size() == 3 && crtNode->myChildren[0]->mySymbol == Symbol::LPAR){
        // exp: LPAR exp RPAR
        expGen(crtNode->myChildren[1]);
    } else{
        // exp: exp OP exp
        expGen(crtNode->myChildren[0]);
        int reg = vm->allocReg();
        printf("addi $t%d $t3 $zero\n", reg);
        expGen(crtNode->myChildren[2]);
        printf("addi $t1 $t%d $zero\n", reg);
        vm->freeReg();
        printf("addi $t2 $t3 $zero\n");
        OpGen(crtNode->myChildren[1]);
    }
}

void OpGen(TreeNode* crtNode){
    Symbol op = crtNode->mySymbol;
    switch (op){
        case Symbol::AND_OP:
            printf("add $t3 $t2 $t1\n");
            break;
        case Symbol::OR_OP:
            printf("or $t3 $t2 $t1\n");
            break;
        case Symbol::PLUS:
            printf("add $t3 $t2 $t1\n");
            break;
        case Symbol::MINUS:
            printf("sub $t3 $t2 $t1\n");
            break;
        case Symbol::MUL_OP:
            printf("mult $t1 $t2\n");
            printf("mfhi $t3\n");
            break;
        case Symbol::DIV_OP:
            printf("div $t1 $t2\n");
            printf("mflo $t3\n");
            break;
        case Symbol::LT:
            printf("slt $t3 $t1 $t2\n");
            printf("bne $t3 $zero 1\n");
            break;
        case Symbol::GT:
            printf("slt $t3 $t2 $t1\n");
            printf("bne $t3 $zero 1\n");
            break;
        case Symbol::EQ:
            printf("beq $1 $2 1\n");
            break;
        case Symbol::NOTEQ:
            printf("bne $1 $2 1\n");
            break;
        case Symbol::LTEQ:
            printf("slt $t3 $t2 $t1\n");
            printf("xori $t3 1\n");
            printf("bne $t3 $zero 1\n");
            break;
        case Symbol::GTEQ:
            printf("slt $t3 $t1 $t2\n");
            printf("xori $t3 1\n");
            printf("bne $t3 $zero 1\n");
            break;
        case Symbol::SHL_OP:
            printf("sllv $t3 $t1 $t2\n");
            break;
        case Symbol::SHR_OP:
            printf("srlv $t3 $t1 $t2\n");
            break;
        case Symbol::ANDAND:
            printf("add $t3 $t2 $t1\n");
            break;
        case Symbol::OROR:
            printf("or $t3 $t2 $t1\n");
            break;
        default:
            printf("Unknown operator\n");
            break;
    }
}