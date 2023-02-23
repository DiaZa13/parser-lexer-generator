//
// Created by Zaray Corado on 2/23/2023.
//

#include "TreeGraph.h"
#include <tuple>

TreeGraph::TreeGraph() = default;;

// if visits an operator y has to create the respective node and the relationship with its children
void TreeGraph::visitOperator(Operators *operators) {
    TreeGraph::count_nodes++;
    std::string p_relation;

    if (operators->getType() == 1){
        auto only = (TreeGraph::nodes.top());
        TreeGraph::nodes.pop();
        p_relation = std::to_string(count_nodes) + "-->" + std::to_string(std::get<1>(only));

    }else if(operators->getType() == 2){
//      gets the left child
        auto left = (TreeGraph::nodes.top());
        TreeGraph::nodes.pop();
//      gets the right child
        auto right (TreeGraph::nodes.top());
        TreeGraph::nodes.pop();
        p_relation = std::to_string(count_nodes) + "-->" + std::to_string(std::get<1>(left)) +"&"+ \
        std::to_string(std::get<1>(right));

    }
    TreeGraph::relations += p_relation;
    TreeGraph::nodes.push(std::make_tuple(operators->getValue(), count_nodes));
    TreeGraph::def_nodes += std::to_string(count_nodes) + "((" + operators->getValue() + "))";
}

// If visits a symbol it only creates a node
void TreeGraph::visitSymbol(Symbols *symbol) {
    TreeGraph::count_nodes++;
    // to find out in an easy way the relationships
    TreeGraph::nodes.push(std::make_tuple(symbol->getValue(), count_nodes));
    TreeGraph::def_nodes += std::to_string(count_nodes) + "((" + symbol->getValue() + "))";
}

std::string TreeGraph::getGraphdata() {
    return TreeGraph::def_nodes + TreeGraph::relations;
}