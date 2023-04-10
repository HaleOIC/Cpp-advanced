#include <string>
#include "./include/address.h"
Address::Address(const std::string& s): address_(s) {
}
std::string Address::address() const {
    return address_;
}