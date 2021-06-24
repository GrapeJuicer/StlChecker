#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class Vec3
{
private:
public:
    float x;
    float y;
    float z;
    Vec3();
    Vec3(float x, float y, float z);
    ~Vec3();
    float length();
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
    Face(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float nx, float ny, float nz);
    ~Face();
};

class Stl
{
private:
    static const int def_comment_byte = 80;
    static const int def_size_byte = 4;
    static const int def_value_byte = 4;
    string comment = "";
    vector<Face> faces;

    Stl();
    ~Stl();
    int size();
    int load(ifstream &file, bool isBinary);
};
