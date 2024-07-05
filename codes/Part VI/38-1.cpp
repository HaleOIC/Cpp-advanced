class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}

private:
    int x_, y_;
};

class Rectangle {
public:
    Rectangle(const Point& topLeft, const Point& bottomRight)
        : topLeft_(topLeft), bottomRight_(bottomRight) {}

private:
    Point topLeft_, bottomRight_;
};
