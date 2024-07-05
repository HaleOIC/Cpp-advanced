#include <iostream>

class Base {
public:
    virtual void foo(int x = 1) {
        std::cout << "Base::foo() called with x=" << x << std::endl;
    }
};

class Derived : public Base {
public:
    virtual void foo(int x = 2) { // 重新定义了缺省参数值
        std::cout << "Derived::foo() called with x=" << x << std::endl;
    }
};

int main() {
    Derived d;
    Base& b = d; // upcast

    b.foo(); // 调用的是 Base::foo()，而不是 Derived::foo()
    return 0;
}
