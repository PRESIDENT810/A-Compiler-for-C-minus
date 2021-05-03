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
    if (crtNode->myChildren[1]->mySymbol != Symbol::nullStr){
        declListGen(crtNode->myChildren[2]);
    }
}

void declGen(TreeNode* crtNode){
    if (crtNode->myChildren.size() == 1){
        vm->registerVar(crtNode->myChildren[0]->semanticValue);
    } else if (crtNode->myChildren.size() == 3){
        Ptr* ptr = vm->registerVar(crtNode->myChildren[0]->semanticValue);
        // TODO: assign value to this address
        int value = atoi(crtNode->myChildren[2]->semanticValue);
        printf("li $v0 %d\n", value);
        printf("sw $v0 %d($fp)\n", ptr->getAddr());
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

void assignStmtGen(TreeNode* crtNode){
    // TODO: implement this shit
    if (crtNode->myChildren.size() == 3){
        // assign a variable
    } else if (crtNode->myChildren.size() == 6){
        // assign an array
    }
}

void ifStmtsGen(TreeNode* crtNode){
    // TODO: implement this shit
    if (crtNode->myChildren.size() == 1){
        // a single IF statement
    } else if (crtNode->myChildren.size() == 3){
        // IF THEN ELSE statement
    }
}

void ifStmtGen(TreeNode* crtNode){

}

void whileStmtGen(TreeNode* crtNode){

}

void dowhileStmtGen(TreeNode* crtNode){

}

void returnStmtGen(TreeNode* crtNode){

}

void rStmtGen(TreeNode* crtNode){

}

void wStmtGen(TreeNode* crtNode){

}

void expGen(TreeNode* crtNode){

}