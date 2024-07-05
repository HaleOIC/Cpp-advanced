#include <iostream>
#include <string>
#include <memory>

#include "./Sharedptr.cpp"

using namespace std;


class CharString {
public:
    CharString() = default;
    CharString(const string& s) {
        ptr = new char[100];                                            // item 16
        int length = s.size();
        for (int i = 0; i < min(length, 100); ++i) ptr[i] = s[i];
        cout << "call the new[]" << endl;
    }
    ~CharString(){
        delete[] ptr;
        cout << "call the delete[]" << endl;                            // item 16
    }
    CharString(const CharString& rhs) {
        for (int i = 0; i < 100; ++i) ptr[i] = rhs.ptr[i];
    }
    CharString& operator=(const CharString& rhs) {
        for (int i = 0; i < 100; ++i) ptr[i] = rhs.ptr[i];
        return *this;
    }
    char* getString() const {
        return ptr;
    }
    void setString(const string &rhs) {
        for (int i = 0; i < 100; ++i) ptr[i] = rhs[i];
    }
private:
    char* ptr;
};
int main(){
    Shareptr<CharString> p1(new CharString("this is the first charstring!"));                   // item 13, 17
    printf("%s\n",p1.get()->getString());
    Shareptr<CharString> p2(new CharString("this is the second charstring!"));
    printf("%s\n",p2.get()->getString());
    p1 = p2;                                                                                    // item 14    
    printf("%s\n",p1.get()->getString());
    cout << "---------------" << endl;
    return 0;
}