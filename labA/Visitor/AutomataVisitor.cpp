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
    Symbols temp_sym = Symbols('<');
    std::shared_ptr<State> start = std::make_shared<State>(State(temp_sym, EPSILON));
    std::shared_ptr<State> end = std::make_shared<State>();
    start->edge_a = end;
    NonDeterministic symbol_auto(temp_sym, start, end);

    this->unionAutom(std::make_unique<NonDeterministic>(symbol_auto), std::move(child));

}

void AutomataVisitor::positiveAutom(std::unique_ptr<NonDeterministic> child) {

    NonDeterministic left;
    left.setSymbols(child->getSymbols());
    std::list<std::shared_ptr<State>> new_states;
//  creates new states same as the child automata
    int count = 1;
    for (auto &x: child->getStates()){
        x->id = count;
        std::shared_ptr<State> a = std::make_shared<State>(State(count,x->symbol, x->flow, x->type));
        new_states.push_back(a);
        count++;
    }
//  set transitions
    for (auto &x: child->getStates()){
        if (x->edge_a != nullptr){
            auto node = std::find_if(std::begin(new_states), std::end(new_states),[&] (const std::shared_ptr<State>& state) { return state->id == x->id;});
            auto edge_a = std::find_if(std::begin(new_states), std::end(new_states),[&] (const std::shared_ptr<State>& state) { return state->id == x->edge_a->id;});
            if(node != new_states.end()){
                (*node)->setEdgeA(*edge_a);
            }
        }
        if (x->edge_b != nullptr){
            auto node = std::find_if(std::begin(new_states), std::end(new_states),[&] (const std::shared_ptr<State>& state) { return state->id == x->id;});
            auto edge_b = std::find_if(std::begin(new_states), std::end(new_states),[&] (const std::shared_ptr<State>& state) { return state->id == x->edge_b->id;});
            if(node != new_states.end()){
                (*node)->setEdgeA(*edge_b);
            }
        }
    }
    left.setState(new_states);

    this->kleeneAutom(std::move(child));
    std::unique_ptr<NonDeterministic> right = std::move(AutomataVisitor::automatas.top());
    this->automatas.pop();

    this->concatenationAutom(std::make_unique<NonDeterministic>(left), std::move(right));

}

void AutomataVisitor::concatenationAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> right) {
    Symbols epsilon('<');
    NonDeterministic concatenation;
//  symbols
    std::set<Symbols> symbols = {epsilon};
    symbols.insert(left->getSymbols().begin(), left->getSymbols().end());
    symbols.insert(right->getSymbols().begin(), right->getSymbols().end());
    concatenation.setSymbols(symbols);
//  initial and accept states
    auto start = left->getStart();
    auto end = right->getEnd();
//  transitions
    for (auto &x:left->getStates()){
        if (x->edge_a == left->getEnd())
            x->edge_a = right->getStart();
        else if (x->edge_b == left->getEnd())
            x->edge_b = right->getStart();
    }
//  save the old states into the new automata
    concatenation.setStates(left->getStates(), right->getStates());
//  TODO delete the unnecessary state
//   concatenation.deleteState(left->getEnd());
//   left->getEnd().reset();
    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(concatenation));
}

void AutomataVisitor::unionAutom(std::unique_ptr<NonDeterministic> left, std::unique_ptr<NonDeterministic> right) {
    Symbols epsilon('<');
    NonDeterministic joining;
//  symbols
    std::set<Symbols> symbols = {epsilon};
    symbols.insert(left->getSymbols().begin(), left->getSymbols().end());
    symbols.insert(right->getSymbols().begin(), right->getSymbols().end());
    joining.setSymbols(symbols);
//  initial and accept states
    auto left_start = left->getStart();
    auto right_start = right->getStart();
    auto left_end = left->getEnd();
    auto right_end = right->getEnd();
    left_end->symbol = epsilon;
    right_end->symbol = epsilon;
    std::shared_ptr<State> start = std::make_shared<State>(State(epsilon, EPSILON));
    std::shared_ptr<State> end = std::make_shared<State>();
//  save the old states into the new automata
    joining.setStates(left->getStates());
    joining.setStates(right->getStates());
    joining.addState(start, end);
//  transitions
    start->edge_a = left_start;
    start->edge_b = right_start;
    left_end->edge_a = end;
    right_end->edge_a = end;

    AutomataVisitor::automatas.push(std::make_unique<NonDeterministic>(joining));
}

void AutomataVisitor::statesIdentifiers(std::shared_ptr<State> start) {
//  TODO Validate to avoid max-depth recursion
    bool exists = std::find(this->check.begin(), this->check.end(), start) != this->check.end();
    if(start != nullptr && !exists){
        this->count_states++;
        start->id = this->count_states;
        this->check.push_back(start);
        statesIdentifiers(start->edge_a);
        statesIdentifiers(start->edge_b);
    }
}

std::string AutomataVisitor::getGraphdata() {
    std::string nodes;
    std::string transitions;
    auto states = std::move(automatas.top());
    this->statesIdentifiers(states->getStates().front());

    for (auto &x:states->getStates()){
        if(x->flow != ACCEPT){
            if (x->edge_a != nullptr){
                nodes += std::to_string(x->id) + "((\"" + std::to_string(x->id) + "\"))\n\t\t";
                transitions += std::to_string(x->id) + "-- " + x->symbol.getValue() + " -->" + std::to_string(x->edge_a->id) + "\n\t\t";
            }
            if (x->edge_b != nullptr)
                transitions += std::to_string(x->id) + "-- " + x->symbol.getValue() + " -->" + std::to_string(x->edge_b->id) + "\n\t\t";
        }else{
            nodes += std::to_string(x->id) + "(((\"" + std::to_string(x->id) + "\")))\n\t\t";
        }
    }
    return nodes + transitions;
}


