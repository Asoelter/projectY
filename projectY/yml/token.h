#ifndef TOKEN_H
#define TOKEN_H

class Token
{
public:
    Token() = default;
    virtual ~Token();

    virtual std::string toString() const = 0;
};

class StringToken : public Token
{
public:
    StringToken(const std::string& value);

    std::string toString() const override;

private:
    std::string value_;
};

class NumericToken : public Token
{
public:
    NumericToken(double value);

    std::string toString() const override;
    double value() const;

private:
    double value_;
};

#endif// TOKEN_H
