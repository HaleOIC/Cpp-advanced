#include <iostream>
#include <map>
using namespace std;

class Day {
public:
    explicit Day() = default;
    explicit Day(const int& d)
         : val(d) {}
    const Day & operator= (const Day& rhs) {
        val = rhs.val;
        return *this;
    }
    Day (const Day& rhs) {
        val = rhs.val;
    }
    int getval() const {
        return val;
    }
private:
    int val;
};

class Month {
public:
    explicit Month() = default;
    static Month Jan() {return Month(1);}
    static Month Feb() {return Month(2);}
    static Month Mar() {return Month(3);}
    static Month Apr() {return Month(4);}
    static Month May() {return Month(5);}
    static Month June() {return Month(6);}
    static Month July() {return Month(7);}
    static Month Aug() {return Month(8);}
    static Month Sep() {return Month(9);}
    static Month Oct() {return Month(10);}
    static Month Nov() {return Month(11);}
    static Month Dec() {return Month(12);}
    Month (const Month& rhs) {
        val = rhs.val;
    }
    const Month& operator=(const Month& rhs) {
        val = rhs.val;
        return *this;
    }
    int getval() const {
        return val;
    }
private:
    explicit Month(int m) : val(m) {}
    int val;
};

class Year {
public:
    explicit Year() = default;
    explicit Year(const int& y) : val(y) {}
    Year (const Year& rhs) {
        val = rhs.val;
    }
    const Year & operator=(const Year& rhs) {
        val = rhs.val;
        return *this;
    }
    int getval() const {
        return val;
    }
private:    
    int val;
};


class Date{
public:
    explicit Date(const Month& m, const Day& d, const Year& y) {
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
            cout << msg << endl;
        }

    }
    int getDate() const {
        return d_.getval();
    }
    int getMonth() const {
        return m_.getval();
    }
    int getYear() const {
        return y_.getval();
    }
private:
    static std::map<int, int> mp;
    static bool check_year(int val) {
        if ((val % 4 == 0 && val % 100 != 0) || (val % 400 == 0)) return true;
        else return false;
    }
    Month m_;
    Day d_;
    Year y_;
};
std::map<int, int> Date::mp{
        {1, 31}, {2, 28}, {3, 31},
        {4, 30}, {5, 31}, {6, 30},
        {7, 31}, {8, 31}, {9, 30},
        {10, 31}, {11, 30}, {12, 31}
};

int main(){
    Date Unit0(Month::Nov(), Day(30), Year(2013));
    Date Unit1(Month::Feb(), Day(29), Year(2019));
    Date Unit2(Month::Feb(), Day(29), Year(2020));
    Date Unit3(Month::Dec(), Day(16), Year(2029));
    return 0;
}