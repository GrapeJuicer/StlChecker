#include "obj.hpp"

using namespace stlset;

Vec3::Vec3() : Vec3(0.0, 0.0, 0.0)
{
}

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3::~Vec3()
{
}

float Vec3::distance(const Vec3 &r) const
{
    return sqrt(pow(this->x - r.x, 2) + pow(this->y - r.y, 2) + pow(this->z - r.z, 2));
}

bool Vec3::inRange(const Vec3 &r, double range, int rule) const
{
    if (rule == rule::component)
    {
        return (fabs(this->x - r.x) <= range && fabs(this->y - r.y) <= range && fabs(this->z - r.z) <= range);
    }
    else if (rule == rule::vertex)
    {
        return (this->distance(r) <= range);
    }
    else
    {
        throw invalid_argument("Invalid rule.");
    }
}

bool Vec3::operator==(const Vec3 &r) const
{
    return (this->x == r.x && this->y == r.y && this->z == r.z);
}

bool Vec3::operator!=(const Vec3 &r) const
{
    return !(*this == r);
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

Face::Face(float x1, float y1, float z1,
           float x2, float y2, float z2,
           float x3, float y3, float z3,
           float nx, float ny, float nz) : Face(Vec3(x1, y1, z1), Vec3(x2, y2, z2), Vec3(x3, y3, z3), Vec3(nx, ny, nz))
{
}

Face::~Face()
{
}

bool Face::inRange(const Face &r, double range, int rule) const
{
    if (rule != rule::component && rule != rule::vertex)
    {
        throw invalid_argument("Invalid rule.");
    }

    for (int i = 0; i < 3; i++)
    {
        if (!this->point[i].inRange(r.point[i], range, rule))
        {
            return false;
        }
    }

    if (!this->normal.inRange(r.normal, range, rule))
    {
        return false;
    }

    return true;
}

bool Face::operator==(const Face &r) const
{
    return (this->normal == r.normal && this->point[0] == r.point[0] && this->point[1] == r.point[1] && this->point[2] == r.point[2]);
}

bool Face::operator!=(const Face &r) const
{
    return !(*this == r);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Stl::Stl()
{
}

Stl::~Stl()
{
}

int Stl::size() const
{
    return (int)this->faces.size();
}

int Stl::load(string file, bool isBinary)
{
    if (isBinary)
    {
        ifstream fs(file, ios::binary);
        if (!fs)
        {
            return -1;
        }

        return this->load(fs, isBinary);
    }
    else
    {
        ifstream fs(file);
        if (!fs)
        {
            return -1;
        }
        return this->load(fs, isBinary);
    }
}

int Stl::load(ifstream &file, bool isBinary)
{
    if (isBinary)
    {
        return this->loadBinary(file);
    }
    else
    {
        return this->loadText(file);
    }
}

int Stl::loadText(ifstream &file)
{
    /*
    solid 任意の文字列
    facet normal x成分値 y成分値 z成分値
    outer loop
    vertex x成分値 y成分値 z成分値
    vertex x成分値 y成分値 z成分値
    vertex x成分値 y成分値 z成分値
    endloop
    endfacet
    facet normal x成分値 y成分値 z成分値
    outer loop
    vertex x成分値 y成分値 z成分値
    vertex x成分値 y成分値 z成分値
    vertex x成分値 y成分値 z成分値
    endloop
    endfacet
    （facet normal行からendfacet行までを１枚の三角形データとして以降繰り返し）
    endsolid 任意の文字列
    */

    string word;

    try
    {
        while (file >> word)
        {
            if (word == "solid")
            {
                // その行を全て読み取る
                getline(file, this->comment);
                if (this->comment.length() <= 1) // 任意文字列がなかったら
                {
                    this->comment = ""; // 空にする
                }
            }
            else if (word == "facet")
            {
                Face face;
                file >> word;                                            // "normal" を捨てる
                file >> face.normal.x >> face.normal.y >> face.normal.z; // normal x, normal y, normal zl;
                file >> word >> word;                                    // "outer", "loop" を捨てる

                for (int i = 0; i < 3; i++)
                {
                    file >> word;                                                  // "vertex" を捨てる
                    file >> face.point[i].x >> face.point[i].y >> face.point[i].z; // 点座標読み込み
                }

                file >> word >> word; // "endloop", "endfacet" すてる

                this->faces.push_back(face); // 面を登録
            }
            else if (word == "endsolid")
            {
                break;
            }
        }
    }
    catch (const exception &e)
    {
        // 何か例外が出たら失敗
        return 1;
    }

    return 0;
}

int Stl::loadBinary(ifstream &file)
{
    try
    {
        // 任意文字列の読み込み
        char s[80];
        int size = 0;

        // 任意文字列の読み込み
        file.read(s, this->def_comment_byte);
        this->comment = s;

        // サイズの読み込み
        file.read(reinterpret_cast<char *>(&size), this->def_size_byte);

        for (int i = 0; i < size; i++)
        {
            Face face;

            // normal position
            file.read(reinterpret_cast<char *>(&face.normal.x), this->def_value_byte);
            file.read(reinterpret_cast<char *>(&face.normal.y), this->def_value_byte);
            file.read(reinterpret_cast<char *>(&face.normal.z), this->def_value_byte);

            // point position
            for (int i = 0; i < 3; i++)
            {
                file.read(reinterpret_cast<char *>(&face.point[i].x), this->def_value_byte);
                file.read(reinterpret_cast<char *>(&face.point[i].y), this->def_value_byte);
                file.read(reinterpret_cast<char *>(&face.point[i].z), this->def_value_byte);
            }

            file.seekg(2, ios::cur); // 2byte 進む

            // 登録
            this->faces.push_back(face);
        }
    }
    catch (const exception &e)
    {
        return -1;
    }

    return 0;
}

void Stl::show() const
{
    cout << "comment: " << this->comment << endl;
    cout << "size: " << this->size() << endl;
    for (int i = 0; i < this->size(); i++)
    {
        this->showItem(i);
    }
}

int Stl::showItem(int index) const
{
    if (index >= this->size())
    {
        cout << "none" << endl;
        return -1;
    }

    cout << "norm:(" << this->faces[index].normal.x << "," << this->faces[index].normal.y << "," << this->faces[index].normal.z << ")\t" << ends;
    cout << "p1:(" << this->faces[index].point[0].x << "," << this->faces[index].point[0].y << "," << this->faces[index].point[0].z << ")\t" << ends;
    cout << "p2:(" << this->faces[index].point[1].x << "," << this->faces[index].point[1].y << "," << this->faces[index].point[1].z << ")\t" << ends;
    cout << "p3:(" << this->faces[index].point[2].x << "," << this->faces[index].point[2].y << "," << this->faces[index].point[2].z << ")" << endl;
    return 0;
}

// now , lv_exactly only
bool Stl::equals(const Stl &r, int lv) const
{
    return this->equalsWithShow(r, false, lv);
}

bool Stl::equalsWithShow(const Stl &r, bool isShow, int lv) const
{
    bool flag = true;
    int lsize, ssize;

    if (this->size() != r.size())
    {
        if (isShow)
        {
            flag = false;
        }
        else
        {
            return false;
        }
    }

    lsize = max<int>(this->size(), r.size());
    ssize = min<int>(this->size(), r.size());

    if (lv == stlset::level::exactly)
    {
        for (int i = 0; i < lsize; i++)
        {
            if (i >= ssize)
            {
                cout << "P" << i + 1 << endl;
                this->showItem(i);
                r.showItem(i);
            }
            else if (this->faces[i] != r.faces[i])
            {
                if (isShow)
                {
                    flag = false;
                    cout << "P" << i + 1 << endl;
                    this->showItem(i);
                    r.showItem(i);
                }
                else
                {
                    return false;
                }
            }
        }

        return flag;
    }
    // else if (lv == stlset::level::pair)
    // {
    //     //
    // }
    // else if(lv == stlset::level::shape)
    // {
    //     //
    // }
    else
    {
        throw invalid_argument("Invalid rule.");
    }
}

bool Stl::inRangeWithShow(const Stl &r, float range, bool isShow, int rule) const
{
    bool flag = true;
    int lsize, ssize;

    if (rule != rule::component && rule != rule::vertex)
    {
        throw invalid_argument("Invalid rule.");
    }

    if (this->size() != r.size())
    {
        if (isShow)
        {
            flag = false;
        }
        else
        {
            return false;
        }
    }

    lsize = max<int>(this->size(), r.size());
    ssize = min<int>(this->size(), r.size());

    for (int i = 0; i < lsize; i++)
    {
        if (i >= ssize)
        {
            cout << "P" << i + 1 << endl;
            this->showItem(i);
            r.showItem(i);
        }
        else if (!this->faces[i].inRange(r.faces[i], range, rule))
        {
            if (isShow)
            {
                flag = false;
                cout << "P" << i + 1 << endl;
                this->showItem(i);
                r.showItem(i);
            }
            else
            {
                return false;
            }
        }
    }

    return flag;
}

bool Stl::operator==(const Stl &r) const
{
    return this->equals(r);
}

bool Stl::operator!=(const Stl &r) const
{
    return !(*this == r);
}
