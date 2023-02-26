//
// Created by Zaray Corado on 2/22/2023.
//

#include "Characters.h"

Symbols::Symbols() = default;;

Symbols::Symbols(char value) {
    this-> value = value;
    id = int(value);
}

Symbols::Symbols(int id) {
    this-> value = ' ';
    id = id;
}

int Symbols::getPrecedence() {
    return 0;
}

int Symbols::getType() {
    if (id == -1)
        return -2;
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



