class Widget {
public:
    Widget() = default;
    Widget(const Widget &rhs);      //copy constructor
    Widget& operator=(const Widget &rhs);   //copy assignment operator
};

int main(){
    Widget w1;
    Widget w2(w1);
    w1 = w2;
    return 0;
}