#include <iostream>

class Base{
public:
    Base(){};
    virtual ~Base() = 0; 
};// an abstract clase
Base::~Base() {
    std::cout << "this is Base destructor" << std::endl;
}

class Foo:public Base{
public:
    Foo(const std::string &s) {
        s_ = new std::string(s);
        val_ = new int(s.size());
    }
    ~Foo(){
        delete s_;
        delete val_;
        std::cout << "this is Foo destructor" << std::endl;
    }
    std::string *get() const{
        return s_;
    }
private:
    int *val_;
    std::string *s_;
};

int main(){
    Foo foo("string_test");
    // auto boo = dynamic_cast<Base*>(&foo);
    std::cout << *foo.get() << std::endl;
    return 0;
}