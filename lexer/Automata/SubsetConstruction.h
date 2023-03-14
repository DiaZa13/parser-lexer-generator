//
// Created by Zaray Corado on 3/10/2023.
//

#ifndef LEXER_SUBSETCONSTRUCTION_H
#define LEXER_SUBSETCONSTRUCTION_H

#include <utility>

#include "../Token/Characters.h"

struct DeterministicState {
    int id = 0;
    FLOW flow;
//  it is only going to save the id of the state
    std::set<std::shared_ptr<State>> states;

    explicit DeterministicState(int id, FLOW flow) {
        this->id = id;
        this->flow = flow;
    };

    explicit DeterministicState(int id, std::set<std::shared_ptr<State>> states) {
        this->id = id;
        this->flow = TRANSITION;
        this->states = states;
    };

};

struct Origin{
    std::shared_ptr<DeterministicState> origin;
//  defines on which symbol occurs the transition
    Symbols symbol;

    Origin(std::shared_ptr<DeterministicState> origin, Symbols symbol){
        this->origin = std::move(origin);
        this->symbol = std::move(symbol);
    }

    bool operator==(const Origin& o) const
    {
        return origin->id == o.origin->id && symbol.getValue() == o.symbol.getValue();
    }

};

// HashFunctions to use in the hashmap transition container
class TransitionHashFunction {
public:
    size_t operator()(const Origin& o) const
    {
        return (std::hash<int>()(o.origin->id)) ^
               (std::hash<char>()(o.symbol.getValue()));
    }
};

class SubsetConstruction {

private:
    int count_states = 0;
    // validates recursion on e-closure
    std::list<std::shared_ptr<State>> check;

    std::set<Symbols> symbols;
    std::list<std::shared_ptr<DeterministicState>> states;
    std::list<std::shared_ptr<DeterministicState>> check_states;
    std::stack<std::shared_ptr<DeterministicState>> temp_states;
    std::unordered_map<Origin, std::shared_ptr<DeterministicState>, TransitionHashFunction> transitions;

public:
    SubsetConstruction(std::unique_ptr<NonDeterministic> afn);

    void setSymbols(const std::set<Symbols> &symbols);
    void setState(std::list<std::shared_ptr<DeterministicState>> states);

//  Subset construction
    std::set<std::shared_ptr<State>> e_closure(const std::shared_ptr<State>&, std::set<std::shared_ptr<State>> temp_states);
    std::set<std::shared_ptr<State>> moveState(const std::set<std::shared_ptr<State>>& origin, Symbols symbol);
    void checkDstates();
    void endStates(const std::shared_ptr<State>& end);

    void cleanCheck();
    static bool compareStates(const std::set<std::shared_ptr<State>>& old, const std::set<std::shared_ptr<State>>& recent);
    std::shared_ptr<DeterministicState> checkExists(const std::set<std::shared_ptr<State>>& old);

//  this is the move for the simulation
    std::shared_ptr<DeterministicState> move(std::shared_ptr<DeterministicState> origin, Symbols symbol);
//  draw the automata
    std::string getGraphdata();

};


#endif //LEXER_SUBSETCONSTRUCTION_H
