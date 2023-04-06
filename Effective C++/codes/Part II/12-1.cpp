/**
 * Explaination
 * class Uncopyable: a virtual class that can not be copied
 * class Server:derive from Uncopyable, using the singel
 * class Customer: the base class for more details
 * class proirityCustomer: derive from Customer
 * 
 * item 8/ item 9 please check out the corresponding code file
 * item 11 will be displayed in the item 29 elegantly
*/
#include <iostream>
#include <string>

using namespace std;

void logCall(const string& funcName) {
    cout << funcName << endl;
}

class Uncopy{
public:
    Uncopy() = default;                                 // item 5
    virtual ~Uncopy() = 0;                              // item 7
private:
    Uncopy (const Uncopy&) = delete;
    Uncopy & operator=(const Uncopy&) = delete;             // item 6
};
Uncopy::~Uncopy() {                                 
    logCall ("Uncopy destructor");
}


class Server: public Uncopy{
public:
    static Server& call();
    string getName() const;
    ~Server() {                                     // item7
        logCall ("Server destructor");
        delete name_;
    }
private:
    Server(const string& name, const size_t &id) 
        : id_(id) {
            logCall ("Server constructor");
            name_ = new string(name);
        }
    string* name_;
    size_t id_;
};
Server& Server::call() {
    static Server sv("Main_server", 0);
    return sv;
}
string Server::getName() const{
    return *name_;
}

class Date: public Uncopy{
public:
    static Date& query();
    string getDate() const;
    ~Date() {
        logCall ("Date destructor");
        delete time_;
    }
private:
    Date(const string& time) {
        logCall ("Date constructor");
        time_ = new string(time);
    }
    string* time_;
};
Date& Date::query() {
    static Date dt("23.04.06");
    return dt;
}
string Date::getDate() const{
    return *time_;
}


class Customer {
public:
    explicit Customer(const string&, Date&, const double&);
    Customer(const Customer&);
    Customer &operator=(const Customer&);
    string getName() const;
    double getMoney() const;
    string getTime() const;
private:
    string name_;
    Date& time_;
    double money_;
};
Customer::Customer(const string& name, Date& time, const double& money) 
    : name_(name), time_(time), money_(money) {
        logCall("Customer constructor");
    }
Customer::Customer(const Customer& rhs) 
    : name_(rhs.name_), time_(rhs.time_), money_(rhs.money_) {
        logCall("Customer copy constructor");
    }
Customer& Customer::operator= (const Customer& rhs) {
    logCall("Customer copy assignment operator");
    name_ = rhs.name_;
    money_ = rhs.money_;
    return *this;                                                           // item 10
}
string Customer::getName() const{
    return name_;
}
double Customer::getMoney() const{
    return money_;
}
string Customer::getTime() const {
    return time_.getDate();
}


class PriorityCustomer: public Customer {
public:
    explicit PriorityCustomer(const string& name, Date& time, const double& money, const int& priority)
        : Customer(name, time, money), priority_(priority) {                        // item 12
            logCall("PriorityCustomer constructor");
        }
    PriorityCustomer (const PriorityCustomer& rhs)
        : Customer(rhs) {
            init(rhs.priority_);
            logCall("PriorityCustomer copy constructor");
        }
    PriorityCustomer &operator= (const PriorityCustomer& rhs) {         
        Customer::operator=(rhs);
        init(rhs.priority_);
        logCall("PriorityCustomer copy assignment operator");
        return *this;                                                               // item 10
    }
    int getPriority() const{
        return priority_;
    }
private:
    void init(const int& pri){
        priority_ = pri;
    }
    int priority_;
};



int main(){
    auto ppc = new PriorityCustomer("First_time", Date::query(), 0.6, 19);
    cout << "----------------------" << endl;
    cout << "Name: " << ppc->getName() << endl;
    cout << "Date: " << ppc->getTime() << endl;
    cout << "money: " << ppc->getMoney() << endl;
    cout << "priority: " << ppc->getPriority() << endl;
    cout << "----------------------" << endl;
    auto pc = dynamic_cast<Customer*>(ppc);
    cout << "Name: " << pc->getName() << endl;
    cout << "Date: " << pc->getTime() << endl;
    cout << "money: " << pc->getMoney() << endl;
    delete ppc;
    
    cout << endl << "----------------------" << endl;
    PriorityCustomer pc1("Second_time", Date::query(), 0.5, 15);
    PriorityCustomer pc2("Third_time", Date::query(), 1.63, 12);
    PriorityCustomer pc3("Forth_time", Date::query(), 3.63, 9);
    cout << "----------------------" << endl;
    pc1 = pc2 = pc3;

    return 0;
}