//
// Created by Zaray Corado on 2/22/2023.
//

#include "Operators.h"

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
    else
        op_type = NONE;
}

int Operators::getPrecedence() {
    return this->precedence;
}

int Operators::getType() {
    return static_cast<int>(op_type);
}

void Operators::accept(Visitant *visitor) {
    visitor->visitOperator(this);
}
