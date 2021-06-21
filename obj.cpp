#include "obj.hpp"

Vec3::Vec3() : Vec3(0.0, 0.0, 0.0)
{
}

Vec3::Vec3(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3::~Vec3()
{
}

double Vec3::length()
{
    return sqrt(std::pow(this->x, 2) + std::pow(this->y, 2) + std::pow(this->z, 2));
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Face::Face() : Face(Vec3(), Vec3(), Vec3(), Vec3())
{
}

Face::Face(Vec3 p[3], Vec3 normal) : Face(p[0], p[1], p[2], normal)
{
}

Face::Face(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 normal)
{
    this->point[0] = p1;
    this->point[1] = p2;
    this->point[2] = p3;
    this->normal = normal;
}

Face::Face(double x1, double y1, double z1,
           double x2, double y2, double z2,
           double x3, double y3, double z3,
           double nx, double ny, double nz) : Face(Vec3(x1, y1, z1), Vec3(x2, y2, z2), Vec3(x3, y3, z3), Vec3(nx, ny, nz))
{
}

Face::~Face()
{
}
