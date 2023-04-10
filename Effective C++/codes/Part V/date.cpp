#include "./include/date.h"
#include <iostream>


Date::Date(const Month& m, const Day& d, const Year& y) {
    try
    {
        int month = m.getval(), year = y.getval(), day = d.getval();
        if (year > 2023 || year <= 0) throw "this year must be wrong!";
        if (check_year(year)) mp[2] = 29;
        else mp[2] = 28;
        if (day > mp[month] || day <= 0) throw "this day must be wrong!";
        d_ = d;
        m_ = m;
        y_ = y;
    } catch (const char* msg) {
        std::cout << msg << std::endl;
    }
}

int Date::day() const {
    return d_.getval();
}

int Date::month() const {
    return m_.getval();
}

int Date::year() const {
    return y_.getval();
}

bool Date::check_year(int val) {
    if ((val % 4 == 0 && val % 100 != 0) || (val % 400 == 0)) return true;
    else return false;
}

std::map<int, int> Date::mp{
        {1, 31}, {2, 28}, {3, 31},
        {4, 30}, {5, 31}, {6, 30},
        {7, 31}, {8, 31}, {9, 30},
        {10, 31}, {11, 30}, {12, 31}
};
