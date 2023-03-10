//
// Created by Zaray Corado on 2/21/2023.
//

#include <string>
#include "ExpressionTree.h"

ExpressionTree::ExpressionTree(std::list<std::unique_ptr<Characters>> expression) {
    for (auto &i: expression) {
        if (i->getType() == -1)
            tree.push(std::make_unique<Node>(std::move(i)));
//      checks if it is a unary operator
        else if(i->getType() == 1){
            std::unique_ptr<Node> operation = std::make_unique<Node>(std::move(i));
//          It only takes out the one element of the stack
            operation->left = std::move(tree.top());
            tree.pop();
            tree.push(std::move(operation));
        }
        else {
            // first get the operands or leaves
            std::unique_ptr<Node> operation = std::make_unique<Node>(std::move(i));
            operation->right = std::move(tree.top());
            tree.pop();
            operation->left = std::move(tree.top());
            tree.pop();
            tree.push(std::move(operation));
        }
    }
}

void ExpressionTree::postOrder(std::unique_ptr<Node> root) {
    if (root != nullptr) {
        postOrder(std::move(root->left));
        postOrder(std::move(root->right));
//      accepts the visit
        root->value->accept(ExpressionTree::visitor);
    }
}

std::string ExpressionTree::graphData() {
    ExpressionTree::postOrder(std::move(tree.top()));
    std::string data = visitor.getGraphdata();
    return data;
}