#ifndef TOKEN_H
#define TOKEN_H

class Token
{
public:
    Token() = default;
    virtual ~Token();

    virtual std::string toString() const = 0;
};

#endif// TOKEN_H
