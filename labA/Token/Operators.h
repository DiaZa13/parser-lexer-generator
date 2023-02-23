//
// Created by Zaray Corado on 2/22/2023.
//

#ifndef LANGUAGE_OPERATORS_H
#define LANGUAGE_OPERATORS_H

#include "Characters.h"

class Operators : public Characters{
private:
    int precedence;
    enum {UNARY = 1, BINARY = 2, NONE = 0} op_type;
public:
    explicit Operators(char value);
    int getPrecedence() override;
    int getType() override;
    void accept(Visitant* visitor) override;
};


#endif //LANGUAGE_OPERATORS_H
