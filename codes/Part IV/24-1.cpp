#include <iostream>
#include <string>
using namespace std;

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1) 
    : numerator_(numerator), denominator_(denominator) {}
    Rational (const Rational& rhs) {
        numerator_ = rhs.numerator_;
        denominator_ = rhs.denominator_;
    }
    const Rational& operator=(const Rational& rhs) {
        numerator_ = rhs.numerator_;
        denominator_ = rhs.denominator_;
        return *this;
    }
    int numerator() const {
        return numerator_;
    }
    int denominator() const {
        return denominator_;
    }
    string value() const {
        string rv =  to_string(numerator_) + "/" + to_string(denominator_);
        return rv;
    }
private:
    int numerator_;
    int denominator_;
};
const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.numerator() * rhs.numerator(),
                     lhs.denominator() * rhs.denominator()); 
}
int main(){
    Rational r1(10, 5);
    cout << (2 * r1).value() << endl;
    return 0;
}