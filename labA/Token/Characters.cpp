//
// Created by Zaray Corado on 2/22/2023.
//

#include "Characters.h"

Characters::Characters() = default;

Characters::Characters(char value) {
    this->value = value;
}

char Characters::getValue() const {
    return this->value;
}
