#include "./person.h"

class RealPerson: public Person{
public:
    explicit RealPerson(const std::string&, const Date&, const Address &);
    virtual std::string name() const;
    virtual std::string brithDate() const;
    virtual std::string address() const;
};
