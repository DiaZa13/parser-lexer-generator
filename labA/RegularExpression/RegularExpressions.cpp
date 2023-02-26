/*
 * Created by Zaray Corado on 2/21/2023.
 *
*/

#include <string>
#include <list>
#include <utility>
#include <cctype>
#include <bits/stdc++.h>
#include "RegularExpressions.h"

RegularExpressions::RegularExpressions(std::string expression) {
    this->expression = std::move(expression);
}

// this changes the operators to struct values
std::list<std::unique_ptr<Characters>> RegularExpressions::preprocess() {
    for (char value: expression) {
        if (!RegularExpressions::pexpression.empty()) {
//          check all the conditions to add  concatenation
//          ab || a*() || a*b || b() || ()c
            if (std::isalpha(RegularExpressions::pexpression.back()->getValue()) && std::isalpha(value) || \
                RegularExpressions::pexpression.back()->getType() == 1 && (value == '(' || std::isalpha(value)) || \
                RegularExpressions::pexpression.back()->getValue() == ')' && (std::isalpha(value) || value == '(') || \
                RegularExpressions::pexpression.back()->getType() == -1 && value == '('){
                RegularExpressions::pexpression.push_back(std::make_unique<Operators>('.'));
            }
        }
        if (value == '*' || value == '?' || value == '+' || value == '|' || value == '(' || value == ')'){
            RegularExpressions::pexpression.push_back(std::make_unique<Operators>(value));
        }
        else if (std::isalpha(value) || value == '<'){
            RegularExpressions::pexpression.push_back(std::make_unique<Symbols>(value));
        }else
            RegularExpressions::pexpression.push_back(std::make_unique<Symbols>(-1));
    }
    return std::move(pexpression);
}

std::list<std::unique_ptr<Characters>> RegularExpressions::toPostfix(std::list<std::unique_ptr<Characters>> expression) {
    std::stack<std::unique_ptr<Characters>> exp_operators;
    std::list<std::unique_ptr<Characters>> result;

    for (auto &i: expression) {
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

    RegularExpressions::pexpression.clear();

    return result;
}

