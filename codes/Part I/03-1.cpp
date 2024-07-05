#include <string>
#include <iostream>

class TextBlock {
public:
    TextBlock (const std::string &s) {
        this->text = s;
    }
    const char& operator[] (std::size_t pos) const {
        return text[pos];
    }
    char& operator[] (std::size_t pos) {
        return text[pos];
    }
    const std::string& get() const{
        return text;
    }
private:
    std::string text;
};

int main(){
    TextBlock tb("Hello");
    tb[0] = 'x';
    std::cout << tb.get() << std::endl;
    const TextBlock ctb("Hello");
    // ctb[0] = 'x';       // this is wrong.
    std::cout << ctb.get() << std::endl;
    return 0;
}