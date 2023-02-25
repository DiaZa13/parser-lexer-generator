//
// Created by Zaray Corado on 2/24/2023.
//
#include "../Token/Characters.h"


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
    std::set<State> accepted = {end};
    kleene.setAccepted(accepted);

//  transitions
    Transition a(start, child->getEpsilon());
    kleene.setTransitions(a, child->getStart());

    Transition b(child->getAcceptedState(), child->getEpsilon());
    kleene.setTransitions(b, end);
    kleene.setTransitions(b, child->getStart());

    Transition c(start, child->getEpsilon());
    kleene.setTransitions(c, end);

    for (auto i: child->getStates()){
        if (i.type == START || i.type == ACCEPT)
            i.type = TRANSITION;
    }

    kleene.setStates(child->getStates());

    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(kleene));
}

void AutomataVisitor::maybeAutom(std::unique_ptr<NonDeterministic> child) {
    count_states ++;
    State start(count_states, START);
    count_states ++;
    State end(count_states, ACCEPT);

    NonDeterministic epsilon_transition(child->getEpsilon().getValue(), start, end);
//    this->concatenationAutom(std::move(child), std::make_unique<NonDeterministic>(epsilon_transition));
}

void AutomataVisitor::positiveAutom(std::unique_ptr<NonDeterministic> child) {

}

void AutomataVisitor::concatenationAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> right) {

}

void AutomataVisitor::unionAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> righ) {

}

std::string AutomataVisitor::getGraphdata() {
    std::string data;
    std::unique_ptr<NonDeterministic> result = std::move(AutomataVisitor::automatas.top());
    for(auto i: result->getTransitions()){
        for (auto state: i.second){
            data += "(" + std::to_string(i.first.origin.id) + ", "+i.first.symbol.getValue() + ") -> " + std::to_string(state.id) + "\n";
        }
    }
    return data;
}