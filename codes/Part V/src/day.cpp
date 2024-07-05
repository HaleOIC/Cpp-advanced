#include "./include/day.h"

Day::Day(const int& d) : val(d) {}

const Day& Day::operator= (const Day& rhs) {
    val = rhs.val;
    return *this;
}

Day::Day (const Day& rhs) {
    val = rhs.val;
}

int Day::getval() const {
    return val;
}