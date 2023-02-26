//
// Created by Zaray Corado on 2/25/2023.
//

#ifndef LANGUAGE_ERRORSHANDLER_H
#define LANGUAGE_ERRORSHANDLER_H

#include <string>
#include <list>
#include "../Token/Characters.h"

enum ERRORS_MSG {INVALID_EXPRESSION, INVALID_OPERATOR, MISSING_PARENTHESIS, INVALID_START_EXPRESSION, MISSING_EXPRESSION};
enum CODE_ERROR {IEPX01, IOPX01, IOPX02, IEPX02, IEPX03 };

enum PARENTHESIS_TYPE {OPEN, CLOSE};

/* ERRORS
 * INVALID_EXPRESSION -> expecting an operand or symbol
 * INVALID_OPERATOR -> expecting a symbol
 * MISSING_PARENTHESIS -> expecting a close/open parenthesis
 * INVALID_START_EXPRESSION -> expecting an open parenthesis or symbol instead got an operator
 * MISSING_EXPRESSION -> expecting an operation or symbol instead got empty parenthesis
*/

/* VALIDATIONS
 * SYMBOL [S] ->  (P, U, B, S)
 * UNARY  [U] ->  (U, S, P)
 * BINARY [B] ->  (S)
 * OPARE  [OP] -> (OP, S)
 * CPARE  [CP] -> (CP, U, S)
 */

struct ErrorC{

    ERRORS_MSG type;
    int index;
    std::string message;
    CODE_ERROR code;

    ErrorC(ERRORS_MSG type, int index, std::string message, CODE_ERROR code){
        this->type = type;
        this->index = index;
        this->message = std::move(message);
        this->code = code;
    }
};

class ErrorsHandler {
private:
    struct Parenthesis{
        PARENTHESIS_TYPE type;
        int index;

        Parenthesis(PARENTHESIS_TYPE type, int index){
            this->type = type;
            this->index = index;
        }
        Parenthesis();
    };
    std::list<ErrorC> errors;
    std::list<Parenthesis> parenthesis;
    std::list<std::unique_ptr<Characters>> temp_expression;
public:
    ErrorsHandler();
    bool checkExpression(std::list<std::unique_ptr<Characters>> expression);
    const std::list<struct ErrorC> &getErrors() const;
    std::list<std::unique_ptr<Characters>> &getTempExpression();
};


#endif //LANGUAGE_ERRORSHANDLER_H
