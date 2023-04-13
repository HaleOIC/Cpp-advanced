#include <iostream>
#include <cassert>

using namespace std;

class Rectangle {
public:
    Rectangle(int h, int w)
        : height_(h), width_(w) {}
    virtual void setHeight(int newHeight) {
        height_ = newHeight;
    }
    virtual void setWidth(int newWidth) {
        width_ = newWidth;
    }
    int height() const {
        return height_;
    }
    int width() const {
        return width_;
    }
    int getArea() const {
        return height_ * width_;
    }
private:
    int height_;
    int width_;
};


class Square: public Rectangle {
public:
    Square(int a) 
        : Rectangle(a, a) {}
    virtual void setHeight(int newHeight) {
        Rectangle::setHeight(newHeight);
        Rectangle::setWidth(newHeight);
    }
    virtual void setWidth(int newWidth) {
        Rectangle::setHeight(newWidth);
        Rectangle::setWidth(newWidth);
    }
    int width() const {
        return Rectangle::width();
    }
};

void makeBigger(Rectangle &r){
    int old_height = r.height();
    r.setHeight(r.width() + 10);
    assert(r.height() == old_height);
}

int main() {
    Square s(10);
    assert(s.width() == s.height());
    makeBigger(s);
    assert(s.width() == s.height());
    return 0;
}