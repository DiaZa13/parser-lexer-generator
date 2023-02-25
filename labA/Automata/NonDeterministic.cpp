//
// Created by Zaray Corado on 2/24/2023.
//
#include <utility>
#include "../Token/Characters.h"

NonDeterministic::NonDeterministic() {
//    NonDeterministic::epsilon = Symbols('<');
//    NonDeterministic::symbols.insert(epsilon);
}

NonDeterministic::NonDeterministic(State start, State end, std::unordered_map<Transition, std::set<State>, Transition::HashFunction> last_transition) {
    NonDeterministic::states = {start, end};
    NonDeterministic::epsilon = Symbols('<');
    NonDeterministic::transitions = std::move(last_transition);
}

NonDeterministic::NonDeterministic(std::unordered_map<Transition, std::set<State>, Transition::HashFunction> left,
                                   std::unordered_map<Transition, std::set<State>, Transition::HashFunction> right) {
    NonDeterministic::epsilon = Symbols('<');
    NonDeterministic::transitions.insert(left.begin(), left.end());
    NonDeterministic::transitions.insert(right.begin(), right.end());

}

NonDeterministic::NonDeterministic(std::unordered_map<Transition, std::set<State>, Transition::HashFunction> left) {
    NonDeterministic::epsilon = Symbols('<');
    NonDeterministic::transitions.insert(left.begin(), left.end());

}

NonDeterministic::NonDeterministic(State start, State end, std::unordered_map<Transition, std::set<State>, Transition::HashFunction> left, std::unordered_map<Transition, std::set<State>, Transition::HashFunction> right) {
    NonDeterministic::states = {start, end};
    NonDeterministic::epsilon = Symbols('<');
    NonDeterministic::transitions.insert(left.begin(), left.end());
    NonDeterministic::transitions.insert(right.begin(), right.end());
}

NonDeterministic::NonDeterministic(char symbol_value, State origin, State end) {
    NonDeterministic::epsilon = Symbols('<');
    Symbols symbol(symbol_value);
    NonDeterministic::symbols = {symbol, epsilon};
    NonDeterministic::start = origin;
    NonDeterministic::states = {origin, end};
    NonDeterministic::accepted = {end};
    Transition transition(origin, symbol);
    NonDeterministic::transitions = {{transition, accepted}};
}

std::set<State> NonDeterministic::move(State origin, Symbols symbol) {
    std::set<State> test = {origin};
    return test;
}

Symbols NonDeterministic::getEpsilon() const {
    return epsilon;
}

NonDeterministic::NonDeterministic(std::set<Symbols> symbols, std::set<State> states, State start,
                                   std::set<State> accepted,
                                   std::unordered_map<Transition, std::set<State>> transitions) {
    NonDeterministic::symbols = std::move(symbols);
    NonDeterministic::states = std::move(states);
    NonDeterministic::accepted = std::move(accepted);
    NonDeterministic::transitions.merge(transitions);

}

State NonDeterministic::getAcceptedState() {
    auto end = next(this->accepted.begin(), 0);
    return *end;
}

void NonDeterministic::deleteState(State state) {
    auto delete_state = NonDeterministic::states.find(state);
    NonDeterministic::states.erase(*delete_state);
}

void NonDeterministic::updateStates(int increment) {
    for (auto x : NonDeterministic::states){
        x.id += increment;
        if (x.type == START)
            this->setStart(x);
        if (x.type == ACCEPT)
            this->setAccepted(x);
    }
}


