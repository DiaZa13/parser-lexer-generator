cmake_minimum_required(VERSION 3.24)
project(Language)

set(CMAKE_CXX_STANDARD 17)

add_executable(main ExpressionTree/expressionTree.cpp RegularExpression/regularExpressions.cpp main.cpp Token/Characters.cpp Token/Symbols.cpp Token/Operators.cpp Visitor/TreeGraph.cpp Visitor/Visitant.cpp Visualization/Mermaid.cpp Automata/Automata.cpp Automata/NonDeterministic.cpp Automata/NonDeterministic.cpp Visitor/AutomataVisitor.cpp Visitor/AutomataVisitor.cpp)
