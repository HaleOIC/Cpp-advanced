#include <iostream>
#include <string>

class Uncopy {
public:
    Uncopy() {}
    virtual ~Uncopy() {}
private:
    Uncopy (const Uncopy&) = delete;
    Uncopy &operator= (const Uncopy&) = delete;
};

template<typename T>
class Shareptr {
public:
    Shareptr() 
        : ptr_(nullptr), ref_count_(nullptr) {}
    explicit Shareptr(T* ptr) 
        : ptr_(ptr), ref_count_(new int(1)) {}
    Shareptr (const Shareptr& rhs) 
        : ptr_(rhs.ptr_), ref_count_(rhs.ref_count_) {
            ++*ref_count_;
        }
    ~Shareptr() {
        if (--*ref_count_ == 0) {
            std::cout << "shared_ptr:  call of the delete" << std::endl;
            delete ptr_;
            delete ref_count_;
        }
    }
    Shareptr & operator=(const Shareptr& rhs) {                     // item 14
        if (this != &rhs) {
            if (--*ref_count_ == 0) {
                std::cout << "shared_ptr:  call of the delete" << std::endl;
                delete ptr_;
                delete ref_count_;
            }
            ptr_ = rhs.ptr_;
            ref_count_ = rhs.ref_count_;
            ++*ref_count_;
        }
        return *this;
    }
    T* operator*() const {                             // item 15
        return ptr_;
    }
    T& operator->() const {                            // item 15
        return *ptr_;
    }
    T* get() const {                                   // item 15
        return ptr_;
    }
private:
    T* ptr_;
    int* ref_count_;
};