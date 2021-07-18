#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>

using namespace std;

namespace stlset
{
    namespace level
    {
        const int exactly = 0; // 完全一致. 頂点の順番が完全に一緒でないといけない
        // const int pair    = 1; // n番目の要素が同じ面ならば頂点の順番が違っても許容する. 面の向き(法線)は一致する必要がある
        // const int shape   = 2; // 最終的な形が同じならば許容．法線の向きは一致する必要がある．
    }

    namespace rule
    {
        const int direction = 0; // 点同士の方向別の距離で計算する(XYZのいずれかが範囲外なら範囲外)
        const int point = 1;     // 点同士の直線距離から計算する
    }

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
        float length() const;
        bool inRange(const Vec3 &r, double range, int rule = stlset::rule::direction) const;

        // operator
        bool operator==(const Vec3 &r) const;
        bool operator!=(const Vec3 &r) const;
    };

    class Face
    {
    private:
    public:
        // field
        Vec3 point[3];
        Vec3 normal;

        // method
        Face();
        Face(Vec3 p[3], Vec3 normal);
        Face(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 normal);
        Face(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float nx, float ny, float nz);
        ~Face();
        bool inRange(const Face &r, double range, int rule = stlset::rule::direction) const;

        // operator
        bool operator==(const Face &r) const;
        bool operator!=(const Face &r) const;
    };

    class Stl
    {
    private:
        int loadText(ifstream &file);
        int loadBinary(ifstream &file);

    public:
        // field
        static const int def_comment_byte = 80;
        static const int def_size_byte = 4;
        static const int def_value_byte = 4;
        string comment = "";
        vector<Face> faces;

        // constructor/destructor
        Stl();
        ~Stl();

        // method
        int size() const;
        int load(string file, bool isBinary);
        int load(ifstream &file, bool isBinary);
        void show() const;
        int showItem(int index) const;
        bool equals(const Stl &r, int lv = stlset::level::exactly) const;
        bool equalsWithShow(const Stl &r, bool isShow, int lv = stlset::level::exactly) const;
        bool inRangeWithShow(const Stl &r, float range, bool isShow, int rule = stlset::rule::direction) const;

        // operator
        bool operator==(const Stl &r) const;
        bool operator!=(const Stl &r) const;
    };
}
