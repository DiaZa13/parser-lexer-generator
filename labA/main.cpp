/*
 * Created by Zaray Corado on 1/23/2023
 * Implementing an expression tree
 * Based on: https://medium.com/swlh/build-binary-expression-tree-in-python-36c04123e57b
 *
*/

#include <iostream>
#include "regularExpressions.h"
#include "expressionTree.h"
using namespace std;

int main() {

    string n_expression, postfix;
    // expression = "1+2-3*4+(5^6)*7/8/9*1+2";

    cout <<"Insert the expression: "<<endl;
    cin >> n_expression;

    // convert to postfix
    regularExpressions regex(n_expression);
    postfix = regex.toPostfix(n_expression);
    cout << "Postfix expression: "<< postfix <<endl;

    // evaluate the expression with an expression tree
    expressionTree ex_tree(postfix);
    ex_tree.getPostorder();

    return 0;
}