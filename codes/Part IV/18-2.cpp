#include <iostream>
#include <memory>
using namespace std;

class Investment{
public:
    Investment(const int& times, const double& profits) 
    : times_(times), profits_(profits) {}
    int getTimes() const {
        return times_;
    }
    double getProfits() const {
        return profits_;
    }
private:
    int times_;
    double profits_;
};

shared_ptr<Investment> createInvestment(int times, double profits){
    return shared_ptr<Investment>(new Investment(times, profits));
}
int main(){
    auto p = createInvestment(5, 100.0);
    cout << p->getTimes() << "   " << p->getProfits() << endl;
    return 0;
}