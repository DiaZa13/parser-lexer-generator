/*
 * Created by Zaray Corado on 2/21/2023.
 *
*/

#include <string>
#include <list>
#include <utility>
#include <cctype>
#include <bits/stdc++.h>
#include "regularExpressions.h"
#include "Symbols.h"
#include "Operators.h"

regularExpressions::regularExpressions(std::string expression) {
    this->expression = std::move(expression);
}

// this changes the operators to struct values
void regularExpressions::preprocess() {
    for (char value: expression) {
        if (!regularExpressions::pexpression.empty()) {
//          check all the conditions to add  concatenation
//          ab || a*() || a*b || b() || ()c
            if (std::isalpha(regularExpressions::pexpression.back()->getValue()) && std::isalpha(value) || \
                regularExpressions::pexpression.back()->getType() == 1 && (value == '(' || std::isalpha(value)) || \
                regularExpressions::pexpression.back()->getType() == -1 && (value=='(' || value==')')){
                regularExpressions::pexpression.push_back(std::make_unique<Operators>('.'));
            }
        }
        if (value == '*' || value == '?' || value == '+' || value == '|' || value == '(' || value == ')'){
            regularExpressions::pexpression.push_back(std::make_unique<Operators>(value));
        }
        else{
            regularExpressions::pexpression.push_back(std::make_unique<Symbols>(value));
        }
    }
}


bool regularExpressions::checkExpression() {
    return true;
}

std::list<std::unique_ptr<Characters>> regularExpressions::toPostfix() {
    std::stack<std::unique_ptr<Characters>> exp_operators;
    std::list<std::unique_ptr<Characters>> result;

    for (auto &i: regularExpressions::pexpression) {
        // evaluates if the scan value is an operand
        if (i->getType() == -1)
            result.push_back(std::move(i));
        // if the scan value is not an operand
        else if (i->getValue() == '(' || exp_operators.empty() || i->getPrecedence() > exp_operators.top()->getPrecedence())
                exp_operators.push(std::move(i));
        else if (i->getValue() == ')') {
            while (exp_operators.top()->getValue() != '(') {
                result.push_back(std::move(exp_operators.top()));
                exp_operators.pop();
            }
            // delete the '('
            exp_operators.pop();
        }
        // evaluates the precedence and association
        else {
            while (!exp_operators.empty() && i->getPrecedence() <= exp_operators.top()->getPrecedence()) {
                result.push_back(std::move(exp_operators.top()));
                exp_operators.pop();
            }
            exp_operators.push(std::move(i));
        }
    }

    // clean the stack
    while (!exp_operators.empty()) {
        result.push_back(std::move(exp_operators.top()));
        exp_operators.pop();
    }

    regularExpressions::pexpression.clear();

    return result;
}

