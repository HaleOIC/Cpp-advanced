#include <iostream>
using namespace std;
class CTextBlock{
public:
    CTextBlock(const char *p) {
        pText = const_cast<char*>(p);
    }
    char & operator[](size_t pos) const{
        return pText[pos];
    }
    inline char *get() const {
        return pText;
    }
private:
    char* pText;
};

int main(){
    char s[] = "Hello";
    const CTextBlock cctb(s);
    char *pc = &cctb[0];

    *pc = 'J';
    printf("%s", cctb.get());
    return 0;
}