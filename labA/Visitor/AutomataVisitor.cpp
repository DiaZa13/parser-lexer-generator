//
// Created by Zaray Corado on 2/24/2023.
//
#include "../Token/Characters.h"
#include <list>

AutomataVisitor::AutomataVisitor() = default;

void AutomataVisitor::visitSymbol(Symbols *symbol) {
//  every time that founds a symbol it has to create an automata that has two states
    count_states ++;
    State start(count_states, START);
    count_states ++;
    State end(count_states, ACCEPT);

    NonDeterministic symbol_auto(symbol->getValue(), start, end);
    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(symbol_auto));
}

void AutomataVisitor::visitOperator(Operators *operators) {
//    validates depende on the operation
    if (operators->getType() == 1){
        std::unique_ptr<NonDeterministic> child = std::move(AutomataVisitor::automatas.top());
        AutomataVisitor::automatas.pop();
//      only takes out an automaton from the stack
        if (operators->getValue() == '*') {
            this->kleeneAutom(std::move(child));
        }
        else if(operators->getValue() == '?') {
            this->maybeAutom(std::move(child));
        }
        else {
            this->positiveAutom(std::move(child));
        }
    }else{
        std::unique_ptr<NonDeterministic> right = std::move(AutomataVisitor::automatas.top());
        AutomataVisitor::automatas.pop();
        std::unique_ptr<NonDeterministic> left = std::move(AutomataVisitor::automatas.top());
        AutomataVisitor::automatas.pop();
        if (operators->getValue() == '.'){
            this->concatenationAutom(std::move(left), std::move(right));
        }else
            this->unionAutom(std::move(left), std::move(right));
    }
}

void AutomataVisitor::kleeneAutom(std::unique_ptr<NonDeterministic> child) {
    count_states ++;
    State start(count_states, START);
    count_states ++;
    State end(count_states, ACCEPT);

    NonDeterministic kleene(start, end, child->getTransitions());

//  symbols
    kleene.setSymbols(child->getSymbols());

//  define start
    kleene.setStart(start);

//  define accepted
    kleene.setAccepted(end);

//  transitions
    Transition a(start, child->getEpsilon());
    kleene.setTransitions(a, child->getStart());

    Transition b(child->getAcceptedState(), child->getEpsilon());
    kleene.setTransitions(b, end);
    kleene.setTransitions(b, child->getStart());

    Transition c(start, child->getEpsilon());
    kleene.setTransitions(c, end);

//  define the new states type
    std::set<State> new_states;
    for (auto i: child->getStates()){
        if (i.type == START || i.type == ACCEPT)
            new_states.insert(State(i.id, TRANSITION));
        else
            new_states.insert(i);
    }

    kleene.setStates(new_states);
    std::destroy(new_states.begin(), new_states.end());

    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(kleene));
}

void AutomataVisitor::maybeAutom(std::unique_ptr<NonDeterministic> child) {
    count_states ++;
    State start(count_states, START);
    count_states ++;
    State end(count_states, ACCEPT);

    NonDeterministic epsilon_transition(child->getEpsilon().getValue(), start, end);
    this->unionAutom(std::move(child), std::make_unique<NonDeterministic>(epsilon_transition));
}

void AutomataVisitor::positiveAutom(std::unique_ptr<NonDeterministic> child) {
    NonDeterministic left(child->getTransitions());
    left.setSymbols(child->getSymbols());
//  states
    int increment = child->getStates().size();
    std::set<State> new_states;
    for(auto x : child->getStates()) {
        count_states ++;
        if (x.type == START){
            State start(count_states, START);
            new_states.insert(start);
            left.setStart(start);
        }else if (x.type == ACCEPT){
            State end(count_states, START);
            new_states.insert(end);
            left.setAccepted(end);
        }else{
            State state(count_states, TRANSITION);
            new_states.insert(state);
        }
    }
    left.setStates(new_states);
    std::destroy(new_states.begin(), new_states.end());

    std::list<Transition> to_delete;
    for(auto i: left.getTransitions()){
        int old = i.first.origin.id;
        State o(old + increment, i.first.origin.type);
        Transition a(o, i.first.symbol);
        for (auto x : i.second){
            State o(x.id + increment, i.first.origin.type);
            left.setTransitions(a, o);
        }
        to_delete.push_back(i.first);
    }

    for (auto x: to_delete){
        left.deleteTransitions(x);
    }
    std::destroy(to_delete.begin(), to_delete.end());

    this->kleeneAutom(std::move(child));
    std::unique_ptr<NonDeterministic> child_a = std::move(AutomataVisitor::automatas.top());
    AutomataVisitor::automatas.pop();

    this->concatenationAutom(std::make_unique<NonDeterministic>(left), std::move(child_a));

}

void AutomataVisitor::concatenationAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> right) {

    NonDeterministic concatenation(left->getTransitions(), right->getTransitions());

//  symbols
    concatenation.setSymbols(left->getSymbols());
    concatenation.setSymbols(right->getSymbols());
    right->deleteState(right->getStart());

//  define start
    concatenation.setStart(left->getStart());

//  define accepted
    concatenation.setAccepted(right->getAcceptedState());

    std::list<Transition> to_delete;

    for(auto i: concatenation.getTransitions()){
        if (i.first.origin.id == right->getStart().id){
            Transition a(left->getAcceptedState(), i.first.symbol);
            for (auto x: i.second){
                concatenation.setTransitions(a, x);
            }
           to_delete.push_back(i.first);
        }
    }

//  delete the last transitions
    for (auto x: to_delete){
        concatenation.deleteTransitions(x);
    }

    //  define the new states type
    std::set<State> new_states;
    for (auto i: left->getStates()){
        if (i.type == ACCEPT)
            new_states.insert(State(i.id, TRANSITION));
        else
            new_states.insert(i);
    }
    for (auto i: right->getStates()){
        if (i.type == START)
            new_states.insert(State(i.id, TRANSITION));
        else
            new_states.insert(i);
    }

    concatenation.setStates(new_states);
    std::destroy(new_states.begin(), new_states.end());

//    count_states--;

    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(concatenation));
}

void AutomataVisitor::unionAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> right) {
    count_states ++;
    State start(count_states, START);
    count_states ++;
    State end(count_states, ACCEPT);
    NonDeterministic joining(start, end, left->getTransitions(), right->getTransitions());

//  symbols
    joining.setSymbols(left->getSymbols());
    joining.setSymbols(right->getSymbols());

//states
    std::set<State> new_states;
    for (auto i: left->getStates()){
        if (i.type == START || i.type == ACCEPT)
            new_states.insert(State(i.id, TRANSITION));
        else
            new_states.insert(i);
    }
    for (auto i: right->getStates()){
        if (i.type == START || i.type == ACCEPT)
            new_states.insert(State(i.id, TRANSITION));
        else
            new_states.insert(i);
    }

    joining.setStates(new_states);
    std::destroy(new_states.begin(), new_states.end());

//  define start
    joining.setStart(start);

//  define accepted
    joining.setAccepted(end);

//transitions
    Transition a(start, left->getEpsilon());
    joining.setTransitions(a, left->getStart());
    joining.setTransitions(a, right->getStart());

    Transition b(left->getAcceptedState(), left->getEpsilon());
    joining.setTransitions(b, end);
    Transition c(right->getAcceptedState(), left->getEpsilon());
    joining.setTransitions(c, end);

    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(joining));
}

std::string AutomataVisitor::getGraphdata() {
    std::string data;
    std::unique_ptr<NonDeterministic> result = std::move(AutomataVisitor::automatas.top());
    // create the nodes
    for (auto x: result->getStates()){
        if (x.type != ACCEPT){
            data += std::to_string(x.id) + "((\"" + std::to_string(x.id) + "\"))\n\t\t";
        }else
            data += std::to_string(x.id) + "(((\"" + std::to_string(x.id) + "\")))\n\t\t";
    }
    for(auto i: result->getTransitions()){
        for (auto state: i.second){
            data += std::to_string(i.first.origin.id) + "-- " + i.first.symbol.getValue() + " -->" + std::to_string(state.id) + "\n\t\t";
        }
    }
    return data;
}