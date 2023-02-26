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

#define RESET   "\033[0m"
#define ERROR     "\033[1m\033[31m"      /* Bold Red */
#define MESSAGE     "\033[31m"      /* Red */
#define INDEX    "\033[4m\033[34m"      /* Blue */
#define stringify( name ) #name

int main(int argc, char* argv[]) {
    string input, data;
    list<unique_ptr<Characters>> expression;
    list<unique_ptr<Characters>> pexpression;

    if(argc>1){
        input = argv[1];
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
                string type;
                 switch(x.type){
                     case INVALID_EXPRESSION: type = "INVALID_EXPRESSION";
                     case INVALID_OPERATOR: type = "INVALID_OPERATOR";
                     case MISSING_PARENTHESIS: type = "MISSING_PARENTHESIS";
                     case INVALID_START_EXPRESSION:  type = "INVALID_START_EXPRESSION";
                     case MISSING_EXPRESSION: type = "MISSING_EXPRESSION";
                 }

                throw std::invalid_argument(ERROR "INVALID_EXPRESSION -> "  INDEX + input.substr(0, x.index-1) + ERROR + \
                input.substr(x.index-1, 1) + INDEX + input.substr(x.index, input.length()) + \
                INDEX + "/Expression:" + to_string(x.index) + RESET MESSAGE" :" + x.message + RESET);
            }
        }

    }else
        throw std::invalid_argument("[MISSING_REGULAR_EXPRESSION] Expected regular expression as commandline argument");

    return 0;
}