#include "./include/RealPerson.h"
RealPerson::RealPerson(const std::string &name, const Date &date, const Address &addr)
: Person(name, date, addr) {}


std::string RealPerson::name() const {
    return pImpl->name();
}

std::string RealPerson::brithDate() const {
    return pImpl->birthDate();
}

std::string RealPerson::address() const {
    return pImpl->address();
}