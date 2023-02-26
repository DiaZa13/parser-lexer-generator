/*
 * Created by Zaray Corado on 1/23/2023
 * Implementing an expression tree
 * Based on: https://medium.com/swlh/build-binary-expression-tree-in-python-36c04123e57b
 *
*/

#include <iostream>
#include "RegularExpression/RegularExpressions.h"
#include "ExpressionTree/ExpressionTree.h"
#include "Visualization/Mermaid.h"
#include "LexerErrors/ErrorsHandler.h"

using namespace std;

int main(int argc, char* argv[]) {
    string input, data;
    list<unique_ptr<Characters>> expression;
    list<unique_ptr<Characters>> pexpression;

    if(argc>1){
        input = argv[1];
        cout << input;
        // convert to postfix
        RegularExpressions regex(input);
        // preprocess the regular expression
        expression = regex.preprocess();
        // check if the expression has any errors
        ErrorsHandler errorsHandler;
        if (errorsHandler.checkExpression(std::move(expression))){
//          once the expression is correct then pass it to postfix
            pexpression = regex.toPostfix(std::move(errorsHandler.getTempExpression()));

            // evaluate the expression with an expression tree
            ExpressionTree ex_tree(std::move(pexpression));
            data = ex_tree.graphData();
            Mermaid graph("flowchart LR", data);
            graph.showGraph();
        }else{
            for (auto &x: errorsHandler.getErrors()){
                throw std::invalid_argument( x.message );
            }
        }

    }else
        throw std::invalid_argument("[MISSING_REGULAR_EXPRESSION] Expected regular expression as line argument");

    return 0;
}