#include <iostream>
#include <cmath> // Include cmath for sqrt

using namespace std;

class Point
{
private:
    static int count;
    double _x;
    double _y;

public:
    // Constructors
    Point(double x, double y) : _x(x), _y(y)
    {
        cout << "Parameterized Constructor." << endl;
        count++;
    }
    Point(const Point &p) : _x(p._x), _y(p._y)
    {
        cout << "Copy Constructor." << endl;
        count++;
    }
    void operator=(const Point &p)
    {
        _x = p._x;
        _y = p._y;
        cout << "Overloaded assignment operator." << endl;
    }
    virtual ~Point()
    {
        cout << "Destructor" << endl;
        count--;
    }
    Point() : _x(0.0), _y(0.0)
    {
        cout << "Default Constructor" << endl;
        count++;
    }

    // Member functions
    void setCoords(double x, double y)
    {
        _x = x;
        _y = y;
    }

    void printCoords()
    {
        cout << "X: " << _x << ", Y: " << _y << endl;
    }

    double distFrom(Point &p)
    {
        double dx = _x - p._x;
        double dy = _y - p._y;
        return sqrt(dx * dx + dy * dy);
    }

    Point midPoint(Point &p)
    {
        cout << "MidPoint ";
        double mx = (_x + p._x) / 2;
        double my = (_y + p._y) / 2;
        return Point(mx, my);
    }
    // Point midPoint(Point &p)
    // {
    //     double mx = (_x + p._x) / 2;
    //     double my = (_y + p._y) / 2;
    //     Point temporaryObject(mx, my);
    //     return move(temporaryObject);
    // }

    // Overloaded + operator
    Point operator+(const Point &p) const
    {
        return Point(_x + p._x, _y + p._y);
    }

    // Static member function to get the count of Point objects
    static int getCount()
    {
        return count;
    }
};

int Point::count = 0;

int main()
{
    Point p1(1.0, 2.0);
    Point p2(3.0, 4.0);
    Point p3 = move(p1.midPoint(p2));

    Point p4 = p3;

    return 0;
}