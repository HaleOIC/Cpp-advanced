#include "./include/person.h"

Person::Person(const std::string &name, const Date &date, const Address &addr)
: pImpl(new PersonImpl(name, date, addr)) {}

