#include <iostream>

using namespace std;

class Shape {
public:
    virtual void draw() = 0;
    virtual void resize(int percentage) = 0;
    virtual ~Shape() {}
};

void Shape::draw() {
    cout << "this is the default draw" << endl;
}

class Rectangular: public Shape {
public:
    virtual void resize(int percentage) {}
    virtual void draw() {
        cout << "this is the Rectangular draw" << endl;
    }
};

class Circle: public Shape {
public:
    virtual void resize(int percentage) {}
    virtual void draw() {
        Shape::draw();
    }
};

int main(){
    auto p1 = new Circle;
    auto p2 = new Rectangular;
    p1->draw();
    p2->draw();
    return 0;
}