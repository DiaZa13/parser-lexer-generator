//
// Created by Zaray Corado on 2/22/2023.
//

#include "Symbols.h"

Symbols::Symbols(char value) {
    this-> value = value;
    id = int(value);
}

int Symbols::getPrecedence() {
    return 0;
}

int Symbols::getType() {
    return -1;
}

void Symbols::accept(Visitant *visitor){
    visitor->visitSymbol(this);
}