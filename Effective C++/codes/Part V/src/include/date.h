#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <map>

#include "day.h"
#include "month.h"
#include "year.h"


class Date{
public:
    explicit Date(const Month& m, const Day& d, const Year& y);
    int day() const;
    int month() const;
    int year() const;
private:
    static std::map<int, int> mp;
    static bool check_year(int val);
    Month m_;
    Day d_;
    Year y_;
};

#endif