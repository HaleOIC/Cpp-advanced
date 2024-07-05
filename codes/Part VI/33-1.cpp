#include <iostream>

using namespace std;

class Base {
public:
    virtual void mf1() = 0;
    virtual void mf1(int) {
        cout << "calling Base: the void mf1(int)" << endl;
    }
    virtual void mf2(){}
    void mf3() {
        cout << "calling Base: the void mf3()" << endl;
    }
    void mf3(double x) {
        cout << "calling Base: the void mf3(double)" << endl;
    }
private:
    int val;
};

class Derived: public Base{
public:
    virtual void mf1() {
        cout << "calling Derived: the void mf1()" << endl;
    }
    virtual void mf1(int val) {
        Base::mf1(val);
    }
    virtual void mf2() {
        cout << "calling Derived: the void mf2()" << endl;
    }
};

int main(){
    Derived d;
    int x = 0;
    d.mf1();
    d.mf1(x);
    d.mf2();
    d.mf3();
    d.mf3(x);
    return 0;
}