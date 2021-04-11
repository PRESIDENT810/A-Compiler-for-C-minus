#include <utility>

#include "DFA.h"

static int DFA_NodeCnt = 0;

DFA_Edge::DFA_Edge(int character, DFA_Node *from, DFA_Node *to) {{
    this->character = character;
    this->from = from;
    this->to = to;
}}

DFA_Node::DFA_Node() {
//    this->token = new char[10];
    this->nodes = std::unordered_set<NFA_Node*>();
    this->edges = std::list<DFA_Edge*>();
    this->stateID = DFA_NodeCnt++;
}

void markAll(std::unordered_set<NFA_Node*> set){
    for (auto node : set){
        node->visited = true;
    }
}

void expandDFA(DFA_Node* crtNode){
    for (auto node : crtNode->nodes){
        node->mySet = crtNode;
    }
    auto transMap = std::unordered_map<char, std::unordered_set<NFA_Node*>*>();
    // find all transitions for the current node set
    for (auto node : crtNode->nodes){
        for (auto edge : node->edges){
            // if the transition is not via null string
            if (edge->character == -1) continue;
            // if the transition leads to a visited node
            // TODO: you have to add the edge from current node to the visited node, instead of ignoring it
            // you have to find a way to find the node set from a node, and add the edge from current set to that set of nodes
            if (edge->to->visited){
                DFA_Edge* dfa_Edge = new DFA_Edge(edge->character, crtNode, edge->to->mySet);
                crtNode->edges.emplace_back(dfa_Edge);
                continue;
            }
            auto iter = transMap.find((char) edge->character);
            // if this is the first occurrence of this transition
            if (iter == transMap.end()){
                auto nodesSet = new std::unordered_set<NFA_Node*>();
                transMap.emplace((char) edge->character, nodesSet);
                nodesSet->emplace(edge->to);
            } else{ // this transition is already added into the map
                iter->second->emplace(edge->to);
            }
        }
    }
    // iterate each transition and get their closure to build the DFA graph
    for (auto iter : transMap){
        char transChar = iter.first;
        auto nodesSet = close(*iter.second);
        // mark all nodes in the new set
        markAll(nodesSet);
        DFA_Node* nxtNode = new DFA_Node();
        nxtNode->nodes = nodesSet;
        DFA_Edge* dfa_Edge = new DFA_Edge(transChar, crtNode, nxtNode);
        crtNode->edges.emplace_back(dfa_Edge);
        expandDFA(nxtNode);
    }
}

std::unordered_set<NFA_Node*> close(std::unordered_set<NFA_Node*> initSet){
    auto res = std::move(initSet);
    auto resCp = res; // note: it's impossible to iterate a STL container while inserting elements
    bool changed = true;
    while (changed){
        changed = false;
        res = resCp;
        for (auto fromState : res){
            for (auto edge : fromState->edges){
                if (edge->character == nullstr && res.find(edge->to) == std::end(res)){
                    resCp.insert(edge->to);
                    changed = true;
                }
            }
        }
    }
    return res;
}

DFA_Node* conversion(NFA* srcNFA){
    NFA_Node* startNode = srcNFA->startNode;
    auto startSet = std::unordered_set<NFA_Node*>();
    startSet.emplace(startNode);
    startSet = close(startSet);
    DFA_Node* root = new DFA_Node();
    root->nodes = startSet;
    // mark all nodes in the start set
    markAll(startSet);
    expandDFA(root);
    return root;
}