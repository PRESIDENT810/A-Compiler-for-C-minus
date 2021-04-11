#include "symbol.h"
#include "list"

class TreeNode{
public:
    Symbol symbol; // what type of symbol this node represents
    std::list<TreeNode> children; // this node's children nodes

};