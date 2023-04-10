#include "./include/PersonImpl.h"

PersonImpl::PersonImpl(const std::string &name, const Date &date, const Address &address)
: name_(name), birthDate_(date), address_(address) {}

std::string PersonImpl::name() const {
    return name_;
}

std::string PersonImpl::birthDate() const {
    return std::to_string(birthDate_.year()) + '.' +
            std::to_string(birthDate_.month()) + '.' + 
             std::to_string(birthDate_.day());
}

std::string PersonImpl::address() const {
    return address_.address();
}