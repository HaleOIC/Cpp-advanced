#include <iostream>
#include <string>

using namespace std;

class DBConnection{
public:
    DBConnection(const DBConnection&) = delete;
    static DBConnection &create() {
        static DBConnection db("this is a inner DB");
        return db;
    }
    void close() {
        throw "this db can not be closed!";
    }
private:
    std::string s_;
    DBConnection(const string& s) : s_(s) {}
};


class DBConn{
public:
    DBConn(DBConnection& db) : db_(db), closed_(false){}
    void close(){
        db_.close();
        closed_ = true;
    }
    ~DBConn(){
        if (!closed_) {
            try{
                db_.close();
            } catch(const char* msg) {
                //function 
                cerr << msg << endl;
            }
        }
    }
private:
    DBConnection& db_;
    bool closed_;
};
int main(){
    DBConn dbc(DBConnection::create());    
    return 0;
}
