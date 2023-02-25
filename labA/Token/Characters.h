//
// Created by Zaray Corado on 2/22/2023.
//

#ifndef LANGUAGE_CHARACTERES_H
#define LANGUAGE_CHARACTERES_H

#include <string>
#include <memory>
#include <stack>
#include <set>
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
enum TYPE {START = 1, TRANSITION = 2, ACCEPT = 3};

struct State{
    int id;
    TYPE type;

    State(){
        this-> id = -1;
        this->type = TRANSITION;
    };

    State(int id, TYPE type){
        this->id = id;
        this->type = type;
    }
    bool operator<(const State &state) const{
        return this->id < state.id;
    }
};

struct Transition{
    State origin;
    Symbols symbol;

    Transition(State state, const Symbols& symbols);

    bool operator==(const Transition& otherTrans) const
    {
        if (this->origin.id == otherTrans.origin.id && this->symbol.getValue() == otherTrans.symbol.getValue()) return true;
        else return false;
    }

    struct HashFunction
    {
        size_t operator()(const Transition& transition) const
        {
            size_t state = std::hash<int>()(transition.origin.id) << 1;
            size_t symb = std::hash<int>()(transition.symbol.getId());
            return state ^ symb;
        }
    };

};

class Automata {
protected:
    std::set<Symbols> symbols;
    std::set<State> states;
    State start;
    std::set<State> accepted;
    std::unordered_map<Transition, std::set<State>, Transition::HashFunction> transitions;

public:
    Automata();
    virtual std::set<State> move(State origin, Symbols symbol) = 0;

    const std::set<Symbols> &getSymbols() const;
    const std::set<State> &getStates();
    const State getStart() const;
    const std::set<State> &getAccepted() const;
    const std::unordered_map<Transition, std::set<State>, Transition::HashFunction> &getTransitions() const;

    void setSymbols(const std::set<Symbols> &symbols);

    void setStates(const std::set<State> &states);

    void setStart(const State &start);

    void setAccepted(const std::set<State> &accepted);

    void setTransitions(Transition &transition, const State &destiny);
};

class NonDeterministic : public Automata{
private:
    Symbols epsilon;
public:
    NonDeterministic();
    NonDeterministic(char symbol_value, State origin, State end);
    NonDeterministic(State start, State end, std::unordered_map<Transition, std::set<State>, Transition::HashFunction> last_transition);
    NonDeterministic(State start, State end, std::unordered_map<Transition, std::set<State>, Transition::HashFunction> left, std::unordered_map<Transition, std::set<State>, Transition::HashFunction> right);
    NonDeterministic(std::set<Symbols> symbols, std::set<State> states, State start, std::set<State> accepted, std::unordered_map<Transition, std::set<State>> transitions);
    std::set<State> move(State origin, Symbols symbol) override;
    State getAcceptedState();
    Symbols getEpsilon() const;
};

class AutomataVisitor : public Visitor{
    int count_states = 0;
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
    std::string getGraphdata() override;
};

#endif //LANGUAGE_CHARACTERES_H
