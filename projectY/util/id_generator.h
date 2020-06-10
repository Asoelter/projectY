#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

struct GloballyUnique {};

template<typename T = GloballyUnique>
class IdGenerator
{
public:
    static size_t generate(){ return ++next; }

private:
    inline static size_t next = 0;
};

using GlobalIdGenerator = IdGenerator<GloballyUnique>;

#endif //ID_GENERATOR_H
