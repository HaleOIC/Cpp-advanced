#include "./include/year.h"

Year::Year(const int& y) : val(y) {}
Year::Year(const Year& rhs) {
    val = rhs.val;
}
const Year& Year::operator=(const Year& rhs) {
    val = rhs.val;
    return *this;
}
int Year::getval() const {
    return val;
}
