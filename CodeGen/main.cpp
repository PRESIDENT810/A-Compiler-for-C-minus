#include "CogeGen.h"

extern std::vector<Rule*> rules;
extern std::vector<First*> Firsts;
extern std::vector<Follow*> Follows;
extern LL1Table* table;

int main(){
    makeRules(&rules);
    makeFirsts(&Firsts, &rules);
    makeFollows(&Follows, &Firsts, &rules);
    table = new LL1Table(&Firsts, &Follows, &rules);
    char filename[30] = "../ScannerOutput.txt";
    std::vector<Token*> tokenVec = tokenScanner(filename);
    try {
        TreeNode* root = recursiveParse(Symbol::program, tokenVec, nullptr);
        fixAST(root);
        showTree(root, 0);
    } catch (std::exception &e){
        printf("%s", e.what());
        printf("Wrong grammar, cannot parse!\n");
    }
    return 0;
}
