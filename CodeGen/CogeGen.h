#ifndef PROJECT3_COGEGEN_H
#define PROJECT3_COGEGEN_H

#include "../Parser/fixTree.h"

void programGen(TreeNode* crtNode);

void varDeclsGen(TreeNode* crtNode);

void varDeclGen(TreeNode* crtNode);

void declListGen(TreeNode* crtNode);

void declGen(TreeNode* crtNode);

void codeBlockGen(TreeNode* crtNode);

void stmtsGen(TreeNode* crtNode);

void stmtGen(TreeNode* crtNode);

void controlStmtGen(TreeNode* crtNode);

void rwStmtGen(TreeNode* crtNode);

void assignStmtGen(TreeNode* crtNode);

void ifStmtsGen(TreeNode* crtNode);

void ifStmtGen(TreeNode* crtNode);

void whileStmtGen(TreeNode* crtNode);

void dowhileStmtGen(TreeNode* crtNode);

void returnStmtGen(TreeNode* crtNode);

void rStmtGen(TreeNode* crtNode);

void wStmtGen(TreeNode* crtNode);

void expGen(TreeNode* crtNode);

void OpGen(TreeNode* crtNode);

#endif //PROJECT3_COGEGEN_H
