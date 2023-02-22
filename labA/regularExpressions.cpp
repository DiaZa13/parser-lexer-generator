/*
 * Created by Zaray Corado on 2/21/2023.
 *
*/

#include <string>
#include <utility>
#include <cctype>
#include <bits/stdc++.h>
#include "regularExpressions.h"


regularExpressions::regularExpressions(std::string expression) {
    this->expression = std::move(expression);
}

int regularExpressions::getPrecedence(char c) {
    if (c == operators[0] || c == operators[1] || c == operators[2])
        return 3;
    else if (c == operators[3])
        return 2;
    else if (c == operators[4])
        return 1;
    return 0;
}

bool regularExpressions::isOperator(char c) {
    return std::find(std::begin(operators), std::end(operators), c) != std::end(operators);
}

bool regularExpressions::checkExpression() {
    return true;
}

std::string regularExpressions::toPostfix(const std::string& infix_expression) {
    std::stack<char> exp_operators;
    std::string result;

// TODO change this to accept more than just one character
    for (char value: infix_expression) {
        // evaluates if the scan value is an operand
        if (std::isalpha(value))
            result += value;

            // if the scan value is not an operand
        else if (value == '(') {
            exp_operators.push(value);

        } else if (value == ')') {
            while (exp_operators.top() != '(') {
                result += exp_operators.top();
                exp_operators.pop();
            }
            // delete the '('
            exp_operators.pop();
        }

            // evaluates the precedence
        else if (exp_operators.empty() || this->getPrecedence(value) > this->getPrecedence(exp_operators.top()))
            exp_operators.push(value);

            // evaluates the precedence and association
        else {
            while (!exp_operators.empty() && this->getPrecedence(value) <= this->getPrecedence(exp_operators.top())) {
                result += exp_operators.top();
                exp_operators.pop();
            }
            exp_operators.push(value);
        }

    }

    // clean the stack
    while (!exp_operators.empty()) {
        result += exp_operators.top();
        exp_operators.pop();
    }

    return result;
}

