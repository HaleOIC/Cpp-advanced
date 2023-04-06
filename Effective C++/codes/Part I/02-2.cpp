#include <iostream>

// class CostEstimate {
// private:
//     static const int Nums_;
//     int Costs_ [Nums_]; 
// };
// const int CostEstimate::Nums_ = 10;


// the above have this error:
// array bound is not an integer constant before ']' token

class CostEstimate {
public:
    enum {Nums_ = 5};
    int Costs_[Nums_];
};

int main(){
    CostEstimate foo;
    std::cout << foo.Nums_ << std::endl;
    // std::cout << &foo.Nums_ << std::endl;
    // can not have the address of the a ENUM variable.
    return 0;
}

