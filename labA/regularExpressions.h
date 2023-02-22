/*
 * Created by Zaray Corado on 2/21/2023.
 * Defining the class of regular expressions
*/

#ifndef LANGUAGE_REGULAREXPRESSIONS_H
#define LANGUAGE_REGULAREXPRESSIONS_H

#include <string>

class regularExpressions {
private:
    char operators[5] = {'*', '+', '?', '.', '|'};
    std::string expression;

public:
    explicit regularExpressions(std::string expression);
    int getPrecedence(char c);
    bool isOperator(char c);
    bool checkExpression();
    std::string toPostfix(const std::string& infix_expression);

};

#endif //LANGUAGE_REGULAREXPRESSIONS_H
