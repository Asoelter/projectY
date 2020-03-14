#ifndef UUID_H
#define UUID_H

#include <string>

class Uuid
{
public:
    Uuid();
    ~Uuid() = default;

    bool operator==(const Uuid& other);
    bool operator!=(const Uuid& other);

    [[nodiscard]]
    std::string to_string() const;

    [[nodiscard]]
    Uuid from_string(const std::string& str) const noexcept;
private:
    UUID value_;
};

#endif //UUID_H