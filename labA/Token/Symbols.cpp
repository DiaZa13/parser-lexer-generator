//
// Created by Zaray Corado on 2/22/2023.
//

#include "Characters.h"

Symbols::Symbols() = default;;

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

void Symbols::accept(Visitor &visitor){
    visitor.visitSymbol(this);
}

int Symbols::getId() const {
    return this->id;
}

bool Symbols::operator<(const Symbols &symbol) const{
    return this->value < symbol.value;
}



