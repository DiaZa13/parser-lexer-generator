//
// Created by Zaray Corado on 3/10/2023.
//

#include "SubsetConstruction.h"
#include <bits/stdc++.h>


SubsetConstruction::SubsetConstruction(std::unique_ptr<NonDeterministic> afn) {
//  define the set of symbols
    setSymbols(afn->getSymbols());

//  e-closure of the first state
    std::shared_ptr<DeterministicState> start = std::make_shared<DeterministicState>(
            DeterministicState(count_states, START));
    this->count_states++;
    start->states = e_closure(afn->getStart(), start->states);

//  recursive stopper
    this->cleanCheck();

    this->temp_states.push(start);
    this->check_states.push_back(start);

//  check the states on the stack and make mark them
    this->checkDstates();

//  create the end states
    this->endStates(afn->getEnd());
}

void SubsetConstruction::checkDstates() {
    while (!temp_states.empty()) {
        std::shared_ptr<DeterministicState> temp_state = temp_states.top();
        temp_states.pop();
//      move
        for (auto &x: this->symbols) {
            std::set<std::shared_ptr<State>> moveStates = moveState(temp_state->states, x);
            std::set<std::shared_ptr<State>> afn_states;
            for (auto &y: moveStates) {
                afn_states = e_closure(y, afn_states);
                this->cleanCheck();
            }
            if (!afn_states.empty()) {
//              check if the state already exists
                std::shared_ptr<DeterministicState> destiny = checkExists(afn_states);
//              add the new transition
                Origin origin(temp_state, x);
                this->transitions.insert(std::make_pair(origin, destiny));
            }
        }
        this->states.push_back(temp_state);
    }
}

void SubsetConstruction::endStates(const std::shared_ptr<State> &end) {
    for (auto &x: this->states) {
        if (x->states.count(end))
            x->flow = ACCEPT;
    }
}


void SubsetConstruction::setSymbols(const std::set<Symbols> &symbols) {
    for (auto &x: symbols) {
        if (x.getValue() != '<') {
            this->symbols.insert(x);
        }
    }
}

std::set<std::shared_ptr<State>>
SubsetConstruction::e_closure(const std::shared_ptr<State> &state, std::set<std::shared_ptr<State>> temp_states) {
    bool exists = std::find(this->check.begin(), this->check.end(), state) != this->check.end();

    if (state != nullptr && !exists) {
        temp_states.insert(state);
        if (state->symbol.getValue() == '<') {
            temp_states.insert(state->edge_a);
            if (state->edge_b != nullptr) {
                temp_states.insert(state->edge_b);
            }
            temp_states = e_closure(state->edge_a, temp_states);
            temp_states = e_closure(state->edge_b, temp_states);
        }
        this->check.push_back(state);
    }

    return temp_states;
}

std::set<std::shared_ptr<State>>
SubsetConstruction::moveState(const std::set<std::shared_ptr<State>> &origin, Symbols symbol) {
    std::set<std::shared_ptr<State>> move;
    for (auto &state: origin) {
        if (state->symbol.getValue() == symbol.getValue()) {
            move.insert(state->edge_a);
        }
    }
    return move;
}

bool SubsetConstruction::compareStates(const std::set<std::shared_ptr<State>> &old,
                                       const std::set<std::shared_ptr<State>> &recent) {
    if (old.size() != recent.size())
        return false;

    std::set<std::shared_ptr<State>> intersect;
    std::set_intersection(old.begin(), old.end(), recent.begin(), recent.end(),
                          std::inserter(intersect, intersect.begin()));
    if (intersect.size() != old.size())
        return false;
    else
        return true;
}

std::shared_ptr<DeterministicState> SubsetConstruction::checkExists(const std::set<std::shared_ptr<State>> &old) {
    for (auto &z: this->check_states) {
        if (compareStates(z->states, old)) {
            return z;
        }
    }
    std::shared_ptr<DeterministicState> x = std::make_shared<DeterministicState>(
            DeterministicState(this->count_states, old));
    this->count_states++;
    this->temp_states.push(x);
    this->check_states.push_back(x);
    return x;
}

void SubsetConstruction::cleanCheck() {
    this->check.clear();
}

std::string SubsetConstruction::getGraphdata() {
    std::string nodes;
    std::string transitions;

    for (auto &x: this->states) {
        if (x->flow != ACCEPT) {
            nodes += std::to_string(x->id) + "((\"" + std::to_string(x->id) + "\"))\n\t\t";
        } else
            nodes += std::to_string(x->id) + "(((\"" + std::to_string(x->id) + "\")))\n\t\t";
    }

    for (auto &x: this->transitions) {
        transitions += std::to_string(x.first.origin->id) + "-- " + x.first.symbol.getValue() + " -->" + std::to_string(x.second->id) +"\n\t\t";
    }

    return nodes + transitions;
}

