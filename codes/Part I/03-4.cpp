#include <iostream>

class TextBlock{
public:
    const char& operator[] (std::size_t pos) const{
        //bounds checking
        //log access data
        //verify data integrity
        return text[pos];
    }
    char& operator[] (std::size_t pos) {
        return 
            const_cast<char&> (
                static_cast<const TextBlock&> (*this)[pos]
            );
            // in order to be in case that 
            // the code into a recursive process.
    }
private:
    char *text;
};