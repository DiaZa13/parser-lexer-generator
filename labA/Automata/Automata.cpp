//
// Created by Zaray Corado on 2/23/2023.
//

#include "../Token/Characters.h"

Automata::Automata() = default;

Transition::Transition(State state, const Symbols& symbols) {
    this->origin = state;
    this->symbol = symbols;
}


const std::set<Symbols> &Automata::getSymbols() const {
    return symbols;
}

const std::set<State> &Automata::getStates(){
    return states;
}

const State Automata::getStart() const{
    return start;
}

const std::set<State> &Automata::getAccepted() const {
    return accepted;
}

const std::unordered_map<Transition, std::set<State>, Transition::HashFunction> &Automata::getTransitions() const {
    return transitions;
}

void Automata::setSymbols(const std::set<Symbols> &symbols) {
    Automata::symbols.insert(symbols.begin(), symbols.end());
}

void Automata::setStates(const std::set<State> &states) {
    Automata::states.insert(states.begin(), states.end());
}

void Automata::setStart(const State &start) {
    Automata::start = start;
}

void Automata::setAccepted(const std::set<State> &accepted) {
    Automata::accepted = accepted;
}

void Automata::setTransitions(Transition &transition, const State &destiny) {
    std::set<State> temp_states = {destiny};
    if (Automata::transitions.find(transition) != Automata::transitions.end()){
        temp_states.insert(Automata::transitions.at(transition).begin(), Automata::transitions.at(transition).end());
        // add the new state to the transition
        Automata::transitions[transition] = temp_states;
    }else{
        Automata::transitions.insert(std::make_pair(transition, temp_states));
    }
}
