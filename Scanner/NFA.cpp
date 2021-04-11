#include "NFA.h"

static int NFA_NodeCnt = 0;

NFA_Edge::NFA_Edge(int character, NFA_Node *from, NFA_Node *to){
    this->character = character;
    this->from = from;
    this->to = to;
}

NFA_Node::NFA_Node(){
    this->stateID = NFA_NodeCnt++;
    this->token = nullptr;
    this->visited = false;
}

void addTransition(int character, NFA_Node* from, NFA_Node* to){
    NFA_Edge* edge = new NFA_Edge(character, from, to);
    from->edges.emplace_back(edge);
}

void addTransition(char* characters, NFA_Node* from, NFA_Node* to){
    for (int i=0; i<strlen(characters); i++){
        char character = characters[i];
        NFA_Edge* edge = new NFA_Edge((int) character, from, to);
        from->edges.emplace_back(edge);
    }
}

NFA::NFA(){
    this->startNode = new NFA_Node();
    this->endNode = new NFA_Node();
    addTransition(nullstr, this->startNode, this->endNode);
}

NFA::NFA(char c){
    this->startNode = new NFA_Node();
    this->endNode = new NFA_Node();
    addTransition(c, this->startNode, this->endNode);
}

NFA::NFA(char *s){
    this->startNode = new NFA_Node();
    this->endNode = new NFA_Node();
    addTransition(s, this->startNode, this->endNode);
}

NFA::NFA(NFA_Node *startNode, NFA_Node *endNode){
    this->startNode = startNode;
    this->endNode = endNode;
}

NFA::~NFA(){
    delete this->startNode;
    delete this->endNode;
}

NFA* orNFA(const std::vector<NFA*>& NFAs){
    NFA_Node* startNode = new NFA_Node();
    NFA_Node* endNode = new NFA_Node();
    for (NFA* currentNFA : NFAs){
        addTransition(nullstr, startNode, currentNFA->startNode);
        addTransition(nullstr, currentNFA->endNode, endNode);
    }
    return new NFA(startNode, endNode);
}

NFA* catNFA(const std::vector<NFA*>& NFAs){
    NFA_Node* startNode = nullptr;
    NFA_Node* endNode = nullptr;
    for (NFA* currentNFA : NFAs){
        if (startNode == nullptr){
            startNode = currentNFA->startNode;
            endNode = currentNFA->endNode;
        } else{
            addTransition(nullstr, endNode, currentNFA->startNode);
            endNode = currentNFA->endNode;
        }
    }
    return new NFA(startNode, endNode);
}

NFA* reptNFA(NFA* currentNFA){
    NFA_Node* startNode = new NFA_Node();
    NFA_Node* endNode = new NFA_Node();
    addTransition(nullstr, startNode, currentNFA->startNode);
    addTransition(nullstr, currentNFA->endNode, endNode);
    addTransition(nullstr, endNode, currentNFA->startNode);
    addTransition(nullstr, startNode, endNode);
    return new NFA(startNode, endNode);
}