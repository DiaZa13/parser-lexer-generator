//
// Created by Zaray Corado on 2/21/2023.
//

#include <string>
#include <iostream>
#include "expressionTree.h"

expressionTree::expressionTree(const std::string& expression) {
    for (char value: expression) {
        if (std::isalpha(value))
            tree.push(new Node(value));
//      checks if it is a unary operator
        else if(value == '*' || value == '?' || value == '+'){
            Node *operation = new Node(value);
//          It only takes out the one element of the stack
            operation->left = tree.top();
            tree.pop();
            tree.push(operation);
        }
        else {
            // first get the operands or leaves
            Node *operation = new Node(value);
            operation->right = tree.top();
            tree.pop();
            operation->left = tree.top();
            tree.pop();
            tree.push(operation);
        }
    }
}

void expressionTree::postOrder(struct Node *root) {
    if (root != nullptr) {
        postOrder(root->left);
        postOrder(root->right);
        std::cout << root->value << std::endl;
    }
}

void expressionTree::getPostorder() {
    expressionTree::postOrder(tree.top());
}