#include "CodeGen/CogeGen.h"
#include "CodeGen/VM.h"
#include "Scanner/Scan.h"

extern inputBuffer* iB;
extern FILE* fp2;

extern std::vector<Rule*> rules;
extern std::vector<First*> Firsts;
extern std::vector<Follow*> Follows;
extern LL1Table* table;
extern VM* vm;

int main(){
    // Scanner
    iB = new inputBuffer();
    prepareDFA();
    char inputFile[15] = "./testFile.c";
    char intermediateFile[30] = "./ScannerOutput.txt";
    fp2 = fopen(intermediateFile, "w");
    FILE *fp1 = nullptr;
    fp1 = fopen(inputFile, "r");
    readFile(fp1);
    fclose(fp2);

    // Paraser
    makeRules(&rules);
    makeFirsts(&Firsts, &rules);
    makeFollows(&Follows, &Firsts, &rules);
    table = new LL1Table(&Firsts, &Follows, &rules);
    std::vector<Token*> tokenVec = tokenScanner(intermediateFile);
    TreeNode* root;
//    root = recursiveParse(Symbol::program, tokenVec, nullptr);
//    fixAST(root);
//    showTree(root, 0);
    try {
        root = recursiveParse(Symbol::program, tokenVec, nullptr);
        fixAST(root);
//        showTree(root, 0);
    } catch (std::exception &e){
        printf("%s", e.what());
        printf("Wrong grammar, cannot parse!\n");
        exit(1);
    }

    // codeGen
    vm = new VM();
    programGen(root);
    
    return 0;
}
