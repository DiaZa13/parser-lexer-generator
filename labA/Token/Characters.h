//
// Created by Zaray Corado on 2/22/2023.
//

#ifndef LANGUAGE_CHARACTERES_H
#define LANGUAGE_CHARACTERES_H

#include <string>
#include <memory>
#include <stack>
#include <set>
#include <list>
#include <unordered_map>

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
    int id{};
public:
    explicit Symbols(char value);
    Symbols();
    int getPrecedence() override;
    int getType() override;
    void accept(Visitor &visitor) override;
    int getId() const;
    bool operator<(const Symbols &symbol) const;
};

class Visitor{
public:
    Visitor();
    virtual void visitOperator(Operators *operators) = 0;
    virtual void visitSymbol(Symbols *symbol) = 0;
    virtual std::string getGraphdata() = 0;
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
    std::string getGraphdata() override;
};

/* ------------------------------  AUTOMATA CLASS DECLARATION ---------------------------- */
enum FLOW {START, TRANSITION, ACCEPT};
enum TYPE {IMPORTANT, EPSILON};

// The state conforms state and transitions
struct State{
    int id = 0;
    Symbols symbol;
    FLOW flow;
    TYPE type;
    std::shared_ptr<State> edge_a;
    std::shared_ptr<State> edge_b;

    explicit State(Symbols &symbol, TYPE type){
        this->symbol = symbol;
        this->flow = START;
        this->type = type;
//      this is the immediately next state
        this->edge_a = nullptr;
        this->edge_b = nullptr;
    };

    explicit State(){
//        this->symbol = symbol;
        this->flow = ACCEPT;
        this->type = EPSILON;
//      this is the immediately next state
        this->edge_a = nullptr;
        this->edge_b = nullptr;
    };
};


class Automata {
protected:
    std::set<Symbols> symbols;
    std::list<std::shared_ptr<State>> states;

public:
    Automata();
    virtual std::shared_ptr<State> move(std::shared_ptr<State> origin, Symbols symbol) = 0;

    [[nodiscard]] const std::set<Symbols> &getSymbols() const;
    std::list<std::shared_ptr<State>> getStates();

    void setSymbols(const std::set<Symbols> &symbols);
    void setStates(std::list<std::shared_ptr<State>> states);
};

class NonDeterministic : public Automata{

public:
    NonDeterministic();
    std::shared_ptr<State> move(std::shared_ptr<State> origin, Symbols symbol) override;
    NonDeterministic(Symbols symbol, std::shared_ptr<State> start, std::shared_ptr<State> end);
    NonDeterministic(std::set<Symbols> symbols, std::list<std::shared_ptr<State>> child_states);
    NonDeterministic(std::set<Symbols> symbols, std::list<std::shared_ptr<State>> left_states, std::list<std::shared_ptr<State>> right_states);

    std::shared_ptr<State> getStart();
    std::shared_ptr<State> getEnd();
    void addState(std::shared_ptr<State> state_start, std::shared_ptr<State> state_end);
};

class AutomataVisitor : public Visitor{
    std::list<std::shared_ptr<State>> check;
    std::stack<std::unique_ptr<NonDeterministic>> automatas;
public:
    AutomataVisitor();
    void visitOperator(Operators *operators) override;
    void visitSymbol(Symbols *symbol) override;
    void kleeneAutom(std::unique_ptr<NonDeterministic> child);
    void maybeAutom(std::unique_ptr<NonDeterministic> child);
    void positiveAutom(std::unique_ptr<NonDeterministic> child);
    void unionAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> righ);
    void concatenationAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> right);
    void statesIdentifiers(std::shared_ptr<State> start, int count);
    std::string getGraphdata() override;
};

#endif //LANGUAGE_CHARACTERES_H
