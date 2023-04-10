#ifndef YEAR_H
#define YEAR_H

class Year {
public:
    explicit Year() = default;
    explicit Year(const int& y);
    Year (const Year& rhs);
    const Year & operator=(const Year& rhs);
    int getval() const;
private:    
    int val;
};
#endif