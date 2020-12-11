// pointers to base class
#include <iostream>
using namespace std;

class Polygon {
protected:
    int width, height, sides;
public:
    void set_values(int a, int b)
    {
        width = a; height = b;
    }

    int getSides()
    {
        return sides;
    }
};

class Rectangle : public Polygon {
public:
    int area()
    {
        return width * height;
    }
};

class Triangle : public Polygon {
public:
    int area()
    {
        return width * height / 2;
    }
};


int main() {

    // instances of user defined derived classes
    Rectangle rect;
    Triangle trgl;

    // pointers to base class
    Polygon* ppoly1 = &rect;
    Polygon* ppoly2 = &trgl;

    Polygon* p;

    p = &rect;
    p->set_values(4, 5);

    cout << static_cast<Rectangle*>(p)->area() << endl;

    p = &trgl;
    p->set_values(4, 5);
    cout << static_cast<Triangle*>(p)->area() << endl;

    // -> is the public member access operator
    //ppoly1->set_values(4, 5);
    //ppoly2->set_values(4, 5);

    //polygon->set_values(4, 5, 3);

    // . is the public member access operator
    cout << rect.area() << '\n';
    cout << trgl.area() << '\n';

    return 0;
}

// TASK: modify the program so that it only uses one pointer,
// whilst still keeping the same functionality

