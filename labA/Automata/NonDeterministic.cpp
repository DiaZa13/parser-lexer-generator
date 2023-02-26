//
// Created by Zaray Corado on 2/24/2023.
//
#include <utility>
#include "../Token/Characters.h"

NonDeterministic::NonDeterministic() {
}

NonDeterministic::NonDeterministic(Symbols symbol, std::shared_ptr<State> start, std::shared_ptr<State> end) {
    //  Constructor for symbols
    this->symbols.insert(symbol);
    this->states.push_front(std::move(start));
    this->states.push_back(std::move(end));
}


NonDeterministic::NonDeterministic(std::set<Symbols> symbols, std::list<std::shared_ptr<State>> child_states) {
//  define epsilon symbol
    Symbols epsilon('<');
    this->symbols = {epsilon};
    this->symbols.insert(symbols.begin(), symbols.end());
    this->setStates(std::move(child_states));
}

NonDeterministic::NonDeterministic(std::set<Symbols> symbols, std::list<std::shared_ptr<State>> left_states,
                                   std::list<std::shared_ptr<State>> right_states) {
    //  define epsilon symbol
    Symbols epsilon('<');
    this->symbols = {epsilon};
    this->symbols.insert(symbols.begin(), symbols.end());
    this->setStates(std::move(left_states));
    this->setStates(std::move(right_states));
}

std::shared_ptr<State> NonDeterministic::move(std::shared_ptr<State> origin, Symbols symbol) {
    return states.back();
}

std::shared_ptr<State> NonDeterministic::getStart() {
    for (auto &x: states){
        if (x->flow == START)
            return x;
    }
    return nullptr;
}

std::shared_ptr<State> NonDeterministic::getEnd() {
    for (auto &x: states){
        if (x->flow == ACCEPT)
            return x;
    }
    return nullptr;
}

void NonDeterministic::addState(std::shared_ptr<State> state_start, std::shared_ptr<State> state_end) {
    states.push_front(state_start);
    states.push_back(state_end);
}



