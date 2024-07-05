#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

class Address {
public:
    explicit Address(const std::string& s);
    std::string address() const;
private:
    std::string address_;
};
#endif