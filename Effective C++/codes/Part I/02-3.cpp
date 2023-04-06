#include <iostream>
using namespace std;
#define CALL_WITH_MAX(a,b) f(((a) > (b)) ? (a) : (b))
inline void f(int x) {
    cout << x << endl;
}
template<typename T>
inline void callWithMax(const T &a, const T &b) {
    f(a > b ? a : b);
}
int main(){
    int a = 5, b = 0;
    CALL_WITH_MAX(++a, b);      // a plus twice
    CALL_WITH_MAX(++a, b + 10); // a only plus once
    cout <<"a:" << a << "  b:" << b << endl;

    a = 5, b = 0;
    callWithMax(++a, b);        // a only plus once.
    callWithMax(++a, b + 10);   // a only plus once.
    cout <<"a:" << a << "  b:" << b << endl;
    return 0;
}