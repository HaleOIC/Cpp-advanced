#include <iostream>
#include "./include/RealPerson.h"
using namespace std;

int main(){
    auto pad = new Address("HUST B503");
    cout << pad->address() << endl;
    auto pp = new RealPerson("Shinehale", Date(Month::July(), Day(30), Year(2003)), Address("HUST C103"));
    cout << "Name:" << pp->name() << endl;
    cout << "BirthDate: " << pp->brithDate() << endl;
    cout << "Address for writing this file: " << pp->address() << endl;
    return 0;
}