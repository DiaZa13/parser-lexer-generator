//
// Created by Zaray Corado on 2/23/2023.
//

#include "../Token/Characters.h"
#include <bits/stdc++.h>


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

void Automata::setStates(std::list<std::shared_ptr<State>> left, std::list<std::shared_ptr<State>> right) {
    for(auto &x: left){
        if (x->flow != START){
            x->flow = TRANSITION;
            this->states.push_back(std::move(x));
        }else
            this->states.push_front(x);
    }
    for(auto &x: right){
        if (x->flow != ACCEPT){
            x->flow = TRANSITION;
            this->states.push_back(std::move(x));
        }else
            this->states.push_back(x);
    }
}

void Automata::deleteState(std::shared_ptr<State> state) {
    auto exists = std::find(this->states.begin(), this->states.end(), state);
    this->states;
}

void Automata::setState(std::list<std::shared_ptr<State>> states) {
    for(auto &x: states){
        x->id = 0;
        if (x->flow == START)
            this->states.push_front(std::move(x));
        else
            this->states.push_back(std::move(x));

    }
}

