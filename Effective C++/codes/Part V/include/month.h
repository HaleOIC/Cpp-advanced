#ifndef MONTH_H
#define MONTH_H

class Month {
public:
    explicit Month() = default;
    static Month Jan(); 
    static Month Feb();
    static Month Mar();
    static Month Apr();
    static Month May();
    static Month June();
    static Month July();
    static Month Aug(); 
    static Month Sep();
    static Month Oct();
    static Month Nov();
    static Month Dec();
    Month (const Month& rhs);
    const Month& operator=(const Month& rhs);
    int getval() const;
private:
    explicit Month(int m);
    int val;
};
#endif