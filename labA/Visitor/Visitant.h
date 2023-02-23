//
// Created by Zaray Corado on 2/23/2023.
//

#ifndef LANGUAGE_VISITANT_H
#define LANGUAGE_VISITANT_H


class Operators;
class Symbols;

class Visitant{
public:
    Visitant();
    virtual void visitOperator(Operators *operators) = 0;
    virtual void visitSymbol(Symbols *symbol) = 0;
};


//class AutomataGraph : public Visitor{
//public:
//    void visitOperator(Operators *operators) override;
//    void visitSymbol(Symbols *symbol) override;
//};


#endif //LANGUAGE_VISITANT_H
