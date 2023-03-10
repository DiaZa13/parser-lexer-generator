//
// Created by Zaray Corado on 2/22/2023.
//

#include "Characters.h"

Operators::Operators(char value) {
    this->value = value;

    if (value == '*' || value == '?' || value == '+')
        precedence = 3;
    else if (value == '.')
        precedence = 2;
    else if (value == '|')
        precedence = 1;
    else
        precedence = 0;

    if (value == '*' || value == '?' || value == '+')
        op_type = UNARY;
    else if (value == '.' || value == '|')
        op_type = BINARY;
    else if (value == '(')
        op_type = OPEN_PARENTHESIS;
    else
        op_type = CLOSE_PARENTHESIS;
}

int Operators::getPrecedence() {
    return this->precedence;
}

int Operators::getType() {
    return static_cast<int>(op_type);
}

void Operators::accept(Visitor &visitor) {
    visitor.visitOperator(this);
}
