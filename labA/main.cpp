/*
 * Created by Zaray Corado on 1/23/2023
 * Implementing an expression tree
 * Based on: https://medium.com/swlh/build-binary-expression-tree-in-python-36c04123e57b
 *
*/

#include <iostream>
#include "Token/regularExpressions.h"
#include "ExpressionTree/expressionTree.h"

using namespace std;

int main() {
    string n_expression, data;
    list<unique_ptr<Characters>> expression;
    // expression = "1+2-3*4+(5^6)*7/8/9*1+2";

    cout <<"Insert the expression: "<<endl;
    cin >> n_expression;

    // convert to postfix
    regularExpressions regex(n_expression);
    regex.preprocess();
    expression = regex.toPostfix();

    // evaluate the expression with an expression tree
    expressionTree ex_tree(std::move(expression));
    data = ex_tree.graphData();
    cout << data;

    return 0;
}