#ifndef DAY_H
#define DAY_H

class Day {
public:
    explicit Day() = default;
    explicit Day(const int& d);
    const Day & operator= (const Day& rhs);
    Day (const Day& rhs);
    int getval() const;
private:
    int val;
};

#endif