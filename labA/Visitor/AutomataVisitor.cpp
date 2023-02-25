//
// Created by Zaray Corado on 2/24/2023.
//
#include "../Token/Characters.h"
#include <list>
#include <bits/stdc++.h>

AutomataVisitor::AutomataVisitor() = default;

void AutomataVisitor::visitSymbol(Symbols *symbol) {
    TYPE type = (symbol->getValue() != '<') ? IMPORTANT : EPSILON;
    Symbols temp_sym = Symbols(symbol->getValue());
    std::shared_ptr<State> start = std::make_shared<State>(State(temp_sym, type));
    std::shared_ptr<State> end = std::make_shared<State>();
    start->edge_a = end;

    NonDeterministic symbol_auto(temp_sym, start, end);
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
    Symbols epsilon('<');
    NonDeterministic kleene;
//  symbols
    std::set<Symbols> symbols = {epsilon};
    symbols.insert(child->getSymbols().begin(), child->getSymbols().end());
    kleene.setSymbols(symbols);
//  initial and accept states
    auto child_start = child->getStart();
    auto child_end = child->getEnd();
    child_end->symbol = epsilon;
    std::shared_ptr<State> start = std::make_shared<State>(State(epsilon, EPSILON));
    std::shared_ptr<State> end = std::make_shared<State>();
    kleene.setStates(child->getStates());
    kleene.addState(start, end);
//  transitions
    start->edge_a = child_start;
    start->edge_b = end;
    child_end->edge_a = end;
    child_end->edge_b = child_start;


    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(kleene));
}

void AutomataVisitor::maybeAutom(std::unique_ptr<NonDeterministic> child) {

}

void AutomataVisitor::positiveAutom(std::unique_ptr<NonDeterministic> child) {

//    this->concatenationAutom(std::make_unique<NonDeterministic>(left), std::move(child_a));

}

void AutomataVisitor::concatenationAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> right) {

//    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(concatenation));
}

void AutomataVisitor::unionAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> right) {


//transitions
//    joining.setTransition(start, left->getEpsilon(), State(left->getStart().id+count, TRANSITION));
//    joining.setTransition(start, left->getEpsilon(), State(right->getStart().id+count, TRANSITION));
//    joining.setTransition(State(left->getAcceptedState().id+count,TRANSITION), left->getEpsilon(), end);
//    joining.setTransition(State(right->getAcceptedState().id+count, TRANSITION), left->getEpsilon(), end);

//    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(joining));
}

void AutomataVisitor::statesIdentifiers(std::shared_ptr<State> start, int count) {
//  TODO Validate to avoid max-depth recursion
    bool exists = std::find(this->check.begin(), this->check.end(), start) != this->check.end();
    if(start != nullptr && !exists){
        count++;
        this->check.push_back(start);
        statesIdentifiers(start->edge_a, count);
        statesIdentifiers(start->edge_b, count);
        start->id = count;
    }
}

std::string AutomataVisitor::getGraphdata() {
    std::string nodes;
    std::string transitions;
    auto states = std::move(automatas.top());
    int count = 0;
    this->statesIdentifiers(states->getStates().front(), count);

    for (auto &x:states->getStates()){
        if(x->flow != ACCEPT){
            nodes += std::to_string(x->id) + "((\"" + std::to_string(x->id) + "\"))\n\t\t";
            transitions += std::to_string(x->id) + "-- " + x->symbol.getValue() + " -->" + std::to_string(x->edge_a->id) + "\n\t\t";
            if (x->edge_b != nullptr)
                transitions += std::to_string(x->id) + "-- " + x->symbol.getValue() + " -->" + std::to_string(x->edge_b->id) + "\n\t\t";
        }else{
            nodes += std::to_string(x->id) + "(((\"" + std::to_string(x->id) + "\")))\n\t\t";
        }
    }
    return nodes + transitions;
}


