#include "./include/month.h"

Month Month::Jan() {return Month(1);}
Month Month::Feb() {return Month(2);}
Month Month::Mar() {return Month(3);}
Month Month::Apr() {return Month(4);}
Month Month::May() {return Month(5);}
Month Month::June() {return Month(6);}
Month Month::July() {return Month(7);}
Month Month::Aug() {return Month(8);}
Month Month::Sep() {return Month(9);}
Month Month::Oct() {return Month(10);}
Month Month::Nov() {return Month(11);}
Month Month::Dec() {return Month(12);}

Month::Month (const Month& rhs) {
    val = rhs.val;
}

const Month& Month::operator=(const Month& rhs) {
    val = rhs.val;
    return *this;
}

int Month::getval() const {
    return val;
}
Month::Month(int m) : val(m) {}