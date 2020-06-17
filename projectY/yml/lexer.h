#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "token.h"

class Lexer
{
public:
    using PolicyTableType = std::map<std::regex, std::unique_ptr<Token>>;

    Lexer(const PolicyTableType& table);

    std::vector<Token> tokenize(const std::string& str);

private:
    PolicyTableType policyTable_;
};

#endif //LEXER_H