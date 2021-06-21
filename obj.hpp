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

class Face
{
private:
public:
    Vec3 point[3];
    Vec3 normal;
    Face();
    Face(Vec3 p[3], Vec3 normal);
    Face(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 normal);
    Face(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double nx, double ny, double nz);
    ~Face();
};

