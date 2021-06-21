#include <iostream>

class Vec3
{
private:
public:
    double x;
    double y;
    double z;
    Vec3();
    Vec3(double x, double y, double z);
    ~Vec3();
    double length();
};

