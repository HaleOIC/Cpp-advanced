#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <memory>
#include "PersonImpl.h"

class Person {
public:
    explicit Person(const std::string&, const Date&, const Address &);
    virtual std::string name() const = 0;
    virtual std::string brithDate() const = 0;
    virtual std::string address() const = 0;
protected:
    std::shared_ptr<PersonImpl> pImpl;
};
#endif