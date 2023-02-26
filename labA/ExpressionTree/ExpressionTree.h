//
// Created by Zaray Corado on 2/21/2023.
//

#ifndef LANGUAGE_EXPRESSIONTREE_H
#define LANGUAGE_EXPRESSIONTREE_H

#include <string>
#include <bits/stdc++.h>
#include "../Token/Characters.h"

class ExpressionTree {

private:
    struct Node {
        std::unique_ptr<Characters> value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(std::unique_ptr<Characters> value) {
            this->value = std::move(value);
            this->left = nullptr;
            this->right = nullptr;
        }
    };

    std::stack<std::unique_ptr<Node>> tree;
    AutomataVisitor visitor;
    void postOrder(std::unique_ptr<Node> root);

public:
    explicit ExpressionTree(std::list<std::unique_ptr<Characters>> expression);
    std::string graphData();
};

#endif //LANGUAGE_EXPRESSIONTREE_H
