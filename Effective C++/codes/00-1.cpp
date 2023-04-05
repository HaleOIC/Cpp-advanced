#include <iostream>

class A{
public:
    A();
};

class B{
public:
    explicit B(int x = 0, bool b = true){
        val_ = x;
        check_ = b;
    }
private:
    int val_;
    bool check_;
};

void doSomething(B bObject){
    return;
}
int main(){
    B bObj1;            //generate a class B objective
    doSomething(bObj1);     // OK!
    B bObj2(28);        // explicit transform
    doSomething(bObj2);         // OK!

    // doSomething(28);        // Wrong! this is an subtle transform

    doSomething(B(28));         //OK!

    return 0;
}