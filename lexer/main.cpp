/*
 * Created by Zaray Corado on 1/23/2023
 * Implementing an expression tree
 *
*/

#include <iostream>
#include "RegularExpression/RegularExpressions.h"
#include "ExpressionTree/ExpressionTree.h"
#include "Visualization/Mermaid.h"
#include "LexerErrors/ErrorsHandler.h"
#include "Automata/SubsetConstruction.h"

using namespace std;

#define RESET   "\033[0m"
#define ERROR     "\033[1m\033[31m"      /* Bold Red */
#define MESSAGE     "\033[31m"      /* Red */
#define INDEX    "\033[4m\033[34m"      /* Blue */

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
//            data = ex_tree.graphData();
//            Mermaid graph("flowchart LR", data);

            SubsetConstruction subset(ex_tree.getAutomata());
            data = subset.getGraphdata();
            Mermaid graph("flowchart LR", data);
        }else{
            for (auto &x: errorsHandler.getErrors()){
                cerr << (ERROR "INVALID_EXPRESSION -> "  INDEX + input.substr(0, x.index) + ERROR + \
                input.substr(x.index, 1) + INDEX + input.substr(x.index+1, (input.length() - 1)) + \
                INDEX + "/Expression:" + to_string(x.index+1) + RESET MESSAGE" :" + x.message + RESET "\n");
            }
            return -1;
        }

    }else{
        cerr << (ERROR "INVALID_EXPRESSION -> " RESET MESSAGE "Expected regular expression as commandline argument RESET");
        return -1;
    }

    return 0;
}