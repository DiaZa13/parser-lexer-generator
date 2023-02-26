//
// Created by Zaray Corado on 2/25/2023.
//

#include "ErrorsHandler.h"

ErrorsHandler::ErrorsHandler() {

}

bool ErrorsHandler::checkExpression(std::list<std::unique_ptr<Characters>> expression) {
    int index_count = 1;
    for (auto &value: expression) {
//          look ahead strategy
        if (!temp_expression.empty()) {
//            check after an open parenthesis ( -> (OP, CP, S)
            if(temp_expression.back()->getValue() == '(' && (value->getType() != -1 && value->getValue() != 4 && value->getType() != 5)){
                ErrorC error(INVALID_OPERATOR,
                                           index_count,
                                           "expecting a symbol instead got an operator or unrecognized character",
                                           IOPX01);
                ErrorsHandler::errors.push_back(error);
            }else if(temp_expression.back()->getValue() == '(' && value->getValue() == ')'){
                ErrorC error(MISSING_EXPRESSION,
                                           index_count,
                                           "expecting an operation or symbol instead got empty parenthesis",
                                           IEPX03);
                ErrorsHandler::errors.push_back(error);
            }else if(temp_expression.back()->getType() == 2 && (value->getType() == 1 || value->getType() == 5)){
//                checking binary operators
                ErrorC error(INVALID_OPERATOR,
                                           index_count,
                                           "expecting a symbol instead got an operator",
                                           IOPX01);
                ErrorsHandler::errors.push_back(error);
            }
        }
        else if (value->getType() != 4 && value->getType() != -1){
            ErrorC error(INVALID_START_EXPRESSION,
                                       index_count,
                                       "expecting an open parenthesis or symbol instead got an operator or unrecognized character",
                                       IEPX02);
            ErrorsHandler::errors.push_back(error);
        }
        if(value->getValue() == -2){
            ErrorC error(INVALID_EXPRESSION,
                                       index_count,
                                       "expecting an operand or symbol",
                                       IEPX01);
            ErrorsHandler::errors.push_back(error);
        }
        if(value->getValue() == '('){
            ErrorsHandler::Parenthesis open(OPEN, index_count);
            ErrorsHandler::parenthesis.push_back(open);
        }else if(value->getValue() == ')'){
            ErrorsHandler::Parenthesis close(CLOSE, index_count);
            ErrorsHandler::parenthesis.push_back(close);
        }
        temp_expression.push_back(std::move(value));
        index_count++;
    }
//    checkout the last element
    if(temp_expression.back()->getType() == 2 || temp_expression.back()->getType() == 4){
        ErrorC error(INVALID_OPERATOR,
                     index_count,
                     "cannot end the expression with a binary operator or open parenthesis",
                     IOPX01);
        ErrorsHandler::errors.push_back(error);
    }

//    check for complete parenthesis
    int open = 0, close = 0, index = 0;
    for (auto &x : ErrorsHandler::parenthesis){
        if(x.type == OPEN){
            open++;
        }else
            close++;
    }
    if (open > close){
        for (auto &x : ErrorsHandler::parenthesis){
            if(x.type == OPEN){
                ErrorC error(MISSING_PARENTHESIS,
                                           x.index,
                                           "missing  close parenthesis",
                                           IOPX02);
                ErrorsHandler::errors.push_back(error);
                break;
            }
        }
    } else if(close > open){
        ErrorsHandler::Parenthesis temp_error(CLOSE, 0);
        for (auto &x : ErrorsHandler::parenthesis){
            if(x.type == CLOSE)
                temp_error = x;
        }ErrorC error(MISSING_PARENTHESIS,
                                   temp_error.index,
                                   "missing open parenthesis",
                                   IOPX02);
        ErrorsHandler::errors.push_back(error);
    }

    return errors.empty();
}

std::list<std::unique_ptr<Characters>> &ErrorsHandler::getTempExpression() {
    return temp_expression;
}

const std::list<ErrorC> &ErrorsHandler::getErrors() const {
    return errors;
}
