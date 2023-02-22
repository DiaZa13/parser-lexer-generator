//
// Created by Zaray Corado on 2/21/2023.
//

#ifndef LANGUAGE_EXPRESSIONTREE_H
#define LANGUAGE_EXPRESSIONTREE_H

#include <string>
#include <bits/stdc++.h>

class expressionTree {

private:
    struct Node {
        char value;
        Node *left;
        Node *right;

        explicit Node(char value) {
            this->value = value;
            this->left = nullptr;
            this->right = nullptr;
        }
    };

    std::stack<Node *> tree;

    void postOrder(struct Node *root);

public:
    explicit expressionTree(const std::string& expression);
    void getPostorder();
};

#endif //LANGUAGE_EXPRESSIONTREE_H
