#ifndef PERSONIMPL_H
#define PERSONIMPL_H

#include <string>

#include "address.h"
#include "date.h"

class PersonImpl {
public:
    PersonImpl(const std::string& name,const Date &date, const Address &address);
    virtual std::string name() const;
    virtual std::string birthDate() const;
    virtual std::string address() const;
private:
    std::string name_;
    Date birthDate_;
    Address address_;
};

#endif