#include <iostream>
#include <list>

template <typename T>
class MySet : private std::list<T> {
public:
    using typename std::list<T>::iterator;

    void insert(const T& value) {
        iterator it = std::find(std::list<T>::begin(), std::list<T>::end(), value);
        if (it == std::list<T>::end() || *it != value) {
            std::list<T>::insert(it, value);
        }
    }

    void erase(const T& value) {
        iterator it = std::find(std::list<T>::begin(), std::list<T>::end(), value);
        if (it != std::list<T>::end() && *it == value) {
            std::list<T>::erase(it);
        }
    }

    bool contains(const T& value) const {
        iterator it = std::find(std::list<T>::begin(), std::list<T>::end(), value);
        return it != std::list<T>::end() && *it == value;
    }

    void print() const {
        for (const T& value : *this) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    MySet<int> mySet;

    mySet.insert(2);
    mySet.insert(1);
    mySet.insert(4);
    mySet.insert(3);

    std::cout << "Set contains 3: " << mySet.contains(3) << std::endl;
    std::cout << "Set contains 5: " << mySet.contains(5) << std::endl;

    mySet.print();

    mySet.erase(2);

    mySet.print();

    return 0;
}
