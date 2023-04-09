#include <vector>

class MyClass {
public:
    void swap(MyClass& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
    }
private:
    std::vector<int> data_;
};

void swap(MyClass& lhs, MyClass& rhs) noexcept {
    lhs.swap(rhs);
}
