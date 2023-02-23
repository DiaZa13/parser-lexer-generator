//
// Created by Zaray Corado on 2/22/2023.
//

#ifndef LANGUAGE_CHARACTERES_H
#define LANGUAGE_CHARACTERES_H

#include <string>
#include <memory>

class Visitant;

class Characters {
protected:
//  the string representation of the character
    char value{};

public:
    Characters();
    explicit Characters(char value);
    [[nodiscard]] char getValue() const;
    virtual int getPrecedence() = 0;
    virtual int getType() = 0;
    virtual void accept(Visitant* visitor) = 0;

};


#endif //LANGUAGE_CHARACTERES_H
