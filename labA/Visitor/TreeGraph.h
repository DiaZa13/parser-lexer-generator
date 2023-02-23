//
// Created by Zaray Corado on 2/23/2023.
//

#ifndef LANGUAGE_TREEGRAPH_H
#define LANGUAGE_TREEGRAPH_H

#include "Visitant.h"
#include <stack>
#include <string>

class TreeGraph : public Visitant{
private:
    int count_nodes = 0;
    std::string relations;
    std::string def_nodes;
    std::stack<std::tuple<char, int>> nodes;
public:
    TreeGraph();
    void visitOperator(Operators *operators) override;
    void visitSymbol(Symbols *symbol) override;
    std::string getGraphdata();
};

#endif //LANGUAGE_TREEGRAPH_H
