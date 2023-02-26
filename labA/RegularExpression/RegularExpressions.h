/*
 * Created by Zaray Corado on 2/21/2023.
 * Defining the class of regular expressions
*/

#ifndef LANGUAGE_REGULAREXPRESSIONS_H
#define LANGUAGE_REGULAREXPRESSIONS_H

#include <string>
#include <list>
#include "../Token/Characters.h"

class RegularExpressions {
private:
    std::string expression;
    std::list<std::unique_ptr<Characters>> pexpression;

public:
    explicit RegularExpressions(std::string expression);
    std::list<std::unique_ptr<Characters>> preprocess();
    std::list<std::unique_ptr<Characters>> toPostfix(std::list<std::unique_ptr<Characters>> expression);

};

#endif //LANGUAGE_REGULAREXPRESSIONS_H
