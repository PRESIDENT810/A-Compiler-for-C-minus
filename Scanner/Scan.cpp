#include "DFA.h"

std::vector<DFA_Node*> headNodes;
std::vector<DFA_Node*> crtNodes;

class inputBuffer{
public:
    char buffer[20];
    int ptr = 0;
    inputBuffer(){
        memset(this->buffer, '\0', 20);
    }
    void fillBuffer(char c){
        this->buffer[this->ptr++] = c;
    }
    void clearBuffer(){
        memset(this->buffer, '\0', 20);
        this->ptr = 0;
    }
    void printBuffer(){
        printf("%s\n", this->buffer);
    }
};

inputBuffer* iB = new inputBuffer();

//
// this function initialize all DFA graphs and set the pointer to current DFA graph nodes to the starting node
//
void prepareDFA(){
    headNodes = std::vector<DFA_Node*>();
    // generate DFA for reserved words
    auto reservedWordNFAs = allReservedWordGen();
    for (auto nfa : reservedWordNFAs){
        headNodes.emplace_back(conversion(nfa));
    }
    // generate DFA for intnum
    auto intnumNFA = intnumGen();
    headNodes.emplace_back(conversion(intnumNFA));
    // generate DFA for symbols
    auto symbolNFAs = allSymbolGen();
    for (auto nfa : symbolNFAs){
        headNodes.emplace_back(conversion(nfa));
    }
    // generate DFA for id
    auto idNFA = idGen();
    headNodes.emplace_back(conversion(idNFA));
    // set pointers to current nodes to the starting nodes
    crtNodes = std::vector<DFA_Node*>();
    for (auto headNode : headNodes){
        crtNodes.emplace_back(headNode);
    }
}

//
// this function find whether a DFA graph node is an ending node
// if yes, then output the corresponding token and return true; if no, then return false
//
bool findMatch(DFA_Node* dfa_node){
    for (auto nodes : dfa_node->nodes){
        if (nodes->token != nullptr){
            iB->printBuffer();
            iB->clearBuffer();
            printf("%s\n", nodes->token);
            return true;
        }
    }
    return false;
}

//
// decide if the next character is blank or equivalent
//
bool isBlank(char c){
    return c == ' ' || c == '\t' || c == '\n';
}

//
// read next character (I separate this function for features that might needed later)
//
inline char readNext(FILE *fp){
    char c = fgetc(fp);
    return c;
}

//
// read this file and output token
//
void readFile(const char* filename){
    FILE *fp = nullptr;
    fp = fopen(filename, "r");

    char c = readNext(fp);
    bool matched[headNodes.size()];
    memset(matched, true, headNodes.size());
    // read the file by characters
    while (c != EOF){
        // if all regex doesn't match, output the current token and reset everything
        bool nonMatched = true;
        // try to match each regex
        for (int i=0; i!=crtNodes.size(); i++){
            bool crtRegMatch = false;
            // if no match, skip current regex
            if (!matched[i]) continue;
            for (auto edge : crtNodes[i]->edges){
                if (edge->character == c){
                    crtNodes[i] = edge->to;
                    crtRegMatch = true;
                    nonMatched = false;
                }
            }
            matched[i] = crtRegMatch;
        }
        // if no regex matches
        if (nonMatched){
            // reset teh matching status to false
            memset(matched, true, headNodes.size());
            // output the current token
            for (int i=0; i!=crtNodes.size(); i++){
                if (findMatch(crtNodes[i])) break;
            }
            // reset the tracking node to initial states
            for (int i=0; i!=crtNodes.size(); i++){
                crtNodes[i] = headNodes[i];
            }
            // and you still have to handle the newest char (not matched any regex yet)
            if (isBlank(c)){
                c = readNext(fp);
            } else{
//                iB->fillBuffer(c);
                continue;
            }
        } else{
            iB->fillBuffer(c);
            c = readNext(fp);
        }
    }
    // when EOF, output the last token
    for (int i=0; i!=crtNodes.size(); i++){
        if (findMatch(crtNodes[i])) break;
    }
}

//
// only for debug purpose
//
void test(){
    auto idNFA = intnumGen();
    auto idDFA = conversion(idNFA);
    return;
}

int main(){
//    test();
    prepareDFA();
    char filename[15] = "./testFile.c";
    readFile(filename);
    return 0;
}