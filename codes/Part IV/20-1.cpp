#include <iostream>

using namespace std;
class Window {
public:
    explicit Window(const string& name, const pair<int, int>& figsize, const string& contents)
    : name_(name), figsize_(figsize), contents_(contents) {}
    string name() const {
        return name_;
    }
    virtual void display() const {
        cout << name_ << endl;
        cout << "(" << figsize_.first << "," << figsize_.second << ")" << endl;
        cout << contents_ << endl;
    }
protected:
    string name_;
    pair<int, int> figsize_;
    string contents_;
};

class WindowWithBorders: public Window {
public:
    explicit WindowWithBorders(const string& name, const pair<int, int>& figsize, 
                                const string& contents, const int& color)
    : Window(name, figsize, contents), color_(color) {}
    virtual void display() const {
        cout << name_ << endl;
        cout << "(" << figsize_.first << "," << figsize_.second << ")" << endl;
        cout << "---------------------------------" << endl << contents_ << endl <<"-----------------------------------" << endl;
    }
private:
    int color_;
};

void printNameAndDisplay(Window w) {
    w.display();
}
void printnameanddisplay(const Window& w) {
    w.display();
}
int main(){
    WindowWithBorders wwb("First Window", make_pair(16, 6), "this is the contents to display", 255);
    printNameAndDisplay(wwb);
    printnameanddisplay(wwb);
    return 0;
}