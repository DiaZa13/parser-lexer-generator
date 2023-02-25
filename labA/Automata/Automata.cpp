//
// Created by Zaray Corado on 2/23/2023.
//

#include "../Token/Characters.h"

Automata::Automata() = default;

std::list<std::shared_ptr<State>> Automata::getStates() {
    return states;
}

const std::set<Symbols> &Automata::getSymbols() const {
    return symbols;
}

void Automata::setStates(std::list<std::shared_ptr<State>> states) {
    for(auto &x: states){
        x->flow = TRANSITION;
        this->states.push_back(std::move(x));
    }
}

void Automata::setSymbols(const std::set<Symbols> &symbols) {
    this->symbols.insert(symbols.begin(), symbols.end());
}