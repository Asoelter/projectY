#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "namespace.h"
#include "token.h"

YML_NAMESPACE_BEGIN

class Lexer
{
public:
    using PolicyTable = std::map<std::regex, std::unique_ptr<Token>>;

    Lexer(const PolicyTable& table);

    std::vector<Token> tokenize(const std::string& str);

private:
    PolicyTable policyTable_;
};

YML_NAMESPACE_END

#endif //LEXER_H