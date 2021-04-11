#ifndef UNTITLED_GENNFA_H
#define UNTITLED_GENNFA_H

#include <list>
#include <vector>
#include <unordered_set>
#include <unordered_map>

//
// we use -1 to represent a null string (lambda)
//
const int nullstr = -1;

class NFA_Node;

class DFA_Node;
class DFA_Edge;

//
// Edge class for NFA that saves a edge where one can traverse from a node to a node via a character
//
class NFA_Edge{
public:
    NFA_Edge(int character, NFA_Node* from, NFA_Node* to);
    int character;
    NFA_Node* from;
    NFA_Node* to;
};

//
// Node class for NFA that saves a node and all its edges,
// by traversing via the edges, one can get to other nodes
// stateID assigned each node a serial number
// for the ending node, it stores the token for current regular expression
//
class NFA_Node{
public:
    NFA_Node();
    char* token;
    std::list<NFA_Edge*> edges;
    int stateID;
    bool visited;
    // TODO: add a pointer to the DFA_Node (node set) it belongs to
    DFA_Node* mySet;
};

//
// NFA class saves a non-deterministic finite automata, which has a start node and an end node
//
class NFA{
public:
    NFA();
    explicit NFA(char c);
    explicit NFA(char* s);
    explicit NFA(NFA_Node* startNode, NFA_Node* endNode);
    ~NFA();

    NFA_Node* startNode;
    NFA_Node* endNode;
};

//
// add a transition between two nodes via a single character
//
void addTransition(int character, NFA_Node* from, NFA_Node* to);

//
// add a transition between two nodes via multiple characters
//
void addTransition(char* characters, NFA_Node* from, NFA_Node* to);

//
// this function do the "or" operation with a series of NFAs
//
NFA* orNFA(const std::vector<NFA*>& NFAs);

//
// this function do the "catenation" operation with a series of NFAs
//
NFA* catNFA(const std::vector<NFA*>& NFAs);

//
// this function do the "repeat" operation with a single NFA
//
NFA* reptNFA(NFA* currentNFA);

#endif //UNTITLED_GENNFA_H
