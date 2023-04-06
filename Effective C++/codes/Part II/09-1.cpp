#include <iostream>
#include <string>

using namespace std;

class Transaction {
public:
    Transaction (const string& logInfo);
    void logTransaction(const string& logInfo) const;
};
Transaction::Transaction(const string& logInfo) {
    logTransaction(logInfo);
}
void Transaction::logTransaction(const string& logInfo) const{
    cout << logInfo << endl;
}

class BuyTransaction: public Transaction {
public:
    BuyTransaction(const string &logInfo, const double &money)
        : Transaction(createLogString(logInfo, money)), logInfo_(logInfo), money_(money) {}
private:
    static string createLogString(const string& s, const double& mn);
    string logInfo_;
    double money_;
};
string BuyTransaction::createLogString(const string& s, const double& mn){
    return s;
}

int main(){
    BuyTransaction bt("buytrans", 0.156);
    return 0;
}