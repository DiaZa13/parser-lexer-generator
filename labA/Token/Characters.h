//
// Created by Zaray Corado on 2/22/2023.
//

#ifndef LANGUAGE_CHARACTERES_H
#define LANGUAGE_CHARACTERES_H

#include <string>
#include <memory>
#include <stack>

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
    virtual void accept(class Visitor &visitor) = 0;

};

class Operators : public Characters{
private:
    int precedence;
    enum {UNARY = 1, BINARY = 2, NONE = 0} op_type;
public:
    explicit Operators(char value);
    int getPrecedence() override;
    int getType() override;
    void accept(Visitor &visitor) override;
};

class Symbols : public Characters{
private:
    int id;
public:
    explicit Symbols(char value);
    int getPrecedence() override;
    int getType() override;
    void accept(Visitor &visitor) override;
};

class Visitor{
public:
    Visitor();
    virtual void visitOperator(Operators *operators) = 0;
    virtual void visitSymbol(Symbols *symbol) = 0;
};

class TreeGraph : public Visitor{
private:
    int count_nodes = 0;
    std::string relations;
    std::string def_nodes;
    std::stack<std::tuple<char, int>> nodes;
public:
    TreeGraph();
    void visitOperator(Operators *operators) override;
    void visitSymbol(Symbols *symbol) override;
    std::string getGraphdata();
};

#endif //LANGUAGE_CHARACTERES_H
