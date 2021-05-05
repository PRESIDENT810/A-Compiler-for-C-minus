#ifndef PROJECT3_COGEGEN_H
#define PROJECT3_COGEGEN_H

#include "../Parser/fixTree.h"

//
// generate MIPS code for program node
//
void programGen(TreeNode* crtNode);

//
// generate MIPS code for varDecls node
//
void varDeclsGen(TreeNode* crtNode);

//
// generate MIPS code for varDecl node
//
void varDeclGen(TreeNode* crtNode);

//
// generate MIPS code for declList node
//
void declListGen(TreeNode* crtNode);

//
// generate MIPS code for decl node
//
void declGen(TreeNode* crtNode);

//
// generate MIPS code for codeBlock node
//
void codeBlockGen(TreeNode* crtNode);

//
// generate MIPS code for stmtsGen node
//
void stmtsGen(TreeNode* crtNode);

//
// generate MIPS code for stmtGen node
//
void stmtGen(TreeNode* crtNode);

//
// generate MIPS code for controlStmtGen node
//
void controlStmtGen(TreeNode* crtNode);

//
// generate MIPS code for rwStmtGen node
//
void rwStmtGen(TreeNode* crtNode);

//
// generate MIPS code for assignStmtGen node
//
void assignStmtGen(TreeNode* crtNode);

//
// generate MIPS code for ifStmtGen node
//
void ifStmtsGen(TreeNode* crtNode);

//
// generate MIPS code for ifStmtGen node
//
void ifStmtGen(TreeNode* crtNode);

//
// generate MIPS code for whileStmtGen node
//
void whileStmtGen(TreeNode* crtNode);

//
// generate MIPS code for dowhileStmtGen node
//
void dowhileStmtGen(TreeNode* crtNode);

//
// generate MIPS code for returnStmtGen node
//
void returnStmtGen(TreeNode* crtNode);

//
// generate MIPS code for rStmtGen node
//
void rStmtGen(TreeNode* crtNode);

//
// generate MIPS code for wStmtGen node
//
void wStmtGen(TreeNode* crtNode);

//
// generate MIPS code for expGen node
//
void expGen(TreeNode* crtNode);

//
// generate MIPS code for OpGen node
//
void OpGen(TreeNode* crtNode);

#endif //PROJECT3_COGEGEN_H
