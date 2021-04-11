#ifndef UNTITLED_DFA_H
#define UNTITLED_DFA_H

#include "regex.h"

class DFA_Node;

//
// Edge class for DFA that saves a edge
// where one can traverse from a set of nodes to another set of nodes via a character
//
class DFA_Edge{
public:
    DFA_Edge(int character, DFA_Node* from, DFA_Node* to);
    int character;
    DFA_Node* from;
    DFA_Node* to;
};

//
// Node class for DFA saves a set of nodes and all its edges,
// by traversing via the edges, one can get to other set of nodes
// stateID assigned each node a serial number
// for the ending set of nodes, it stores the token for current regular expression
//
class DFA_Node{
public:
    DFA_Node();
//    char* token;
    std::unordered_set<NFA_Node*> nodes;
    std::list<DFA_Edge*> edges;
    int stateID;
};

//
// this function recursively finds all non-lambda transition of the nodes in the current DFA
// and saves the transition in a vector
// it then use close function to find the set of nodes that is reachable via this transition
// and use recursion to expand that set of nodes
//
void expandDFA(DFA_Node* crtNode);

//
// this function mark all nodes in the set as "visited"
//
void markAll(std::unordered_set<NFA_Node*> set);

//
// this function returns the epi-closure or a set of nodes
//
std::unordered_set<NFA_Node*> close(std::unordered_set<NFA_Node*> initSet);

//
// this function convert a NFA to a DFA head node
//
DFA_Node* conversion(NFA* srcNFA);

#endif //UNTITLED_DFA_H
