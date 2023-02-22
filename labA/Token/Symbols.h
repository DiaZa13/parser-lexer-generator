//
// Created by Zaray Corado on 2/22/2023.
//

#ifndef LANGUAGE_SYMBOLS_H
#define LANGUAGE_SYMBOLS_H

#include "Characters.h"

class Symbols : public Characters{
private:
    int id;
public:
    explicit Symbols(char value);
    int getPrecedence() override;
    int getType() override;
};


#endif //LANGUAGE_SYMBOLS_H
