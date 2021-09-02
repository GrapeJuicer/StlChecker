#include "obj.hpp"

using namespace stlset;

Vec3::Vec3() : Vec3(0.0, 0.0, 0.0)
{
}

Vec3::Vec3(float x, float y, float z)
{
    this->setX(x);
    this->setY(y);
    this->setZ(z);
}

Vec3::~Vec3()
{
}

float Vec3::distance(const Vec3 &r) const
{
    return sqrt(pow(this->getX() - r.getX(), 2) + pow(this->getY() - r.getY(), 2) + pow(this->getZ() - r.getZ(), 2));
}

bool Vec3::inRange(const Vec3 &r, double range, int rule) const
{
    if (rule == rule::component)
    {
        return (abs(this->getX() - r.getX()) <= range && abs(this->getY() - r.getY()) <= range && abs(this->getZ() - r.getZ()) <= range);
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

float Vec3::getX() const
{
    return this->x;
}

void Vec3::setX(float x)
{
    this->x = x;
}

float Vec3::getY() const
{
    return this->y;
}

void Vec3::setY(float y)
{
    this->y = y;
}

float Vec3::getZ() const
{
    return this->z;
}

void Vec3::setZ(float z)
{
    this->z = z;
}

bool Vec3::operator==(const Vec3 &r) const
{
    return (this->getX() == r.getX() && this->getY() == r.getY() && this->getZ() == r.getZ());
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
    this->setVertex(p1, 0);
    this->setVertex(p2, 1);
    this->setVertex(p3, 2);
    this->setNormal(normal);
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
        if (!this->getVertex(i).inRange(r.getVertex(i), range, rule))
        {
            return false;
        }
    }

    if (!this->getNormal().inRange(r.getNormal(), range, rule))
    {
        return false;
    }

    return true;
}

void Face::setVertex(const Vec3 &vertex, unsigned int index)
{
    if (index > 2)
    {
        throw invalid_argument("Invalid index value.");
    }
    this->vertex[index] = vertex;
}

Vec3 Face::getVertex(unsigned int index) const
{
    if (index > 2)
    {
        throw invalid_argument("Invalid index value.");
    }
    return this->vertex[index];
}

void Face::setNormal(const Vec3 &normal)
{
    this->normal = normal;
}

Vec3 Face::getNormal() const
{
    return this->normal;
}

bool Face::operator==(const Face &r) const
{
    return (this->getNormal() == r.getNormal() &&
            this->getVertex(0) == r.getVertex(0) &&
            this->getVertex(1) == r.getVertex(1) &&
            this->getVertex(2) == r.getVertex(2));
}

bool Face::operator!=(const Face &r) const
{
    return !(*this == r);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Stl::Stl()
{
    this->comment = "";
}

Stl::~Stl()
{
}

int Stl::size() const
{
    return (int)this->getFace().size();
}

int Stl::load(string file, bool isBinary)
{
    ifstream fs;

    if (isBinary)
    {
        fs = ifstream(file, ios::binary);
    }
    else
    {
        fs = ifstream(file);
    }

    if (!fs)
    {
        return -1;
    }

    return this->load(fs, isBinary);
}

int Stl::load(ifstream &file, bool isBinary)
{
    return (isBinary ? this->loadBinary(file) : this->loadText(file));
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

    string word, buf;

    try
    {
        while (file >> word)
        {
            if (word == "solid")
            {
                // その行を全て読み取る
                getline(file, buf);
                this->setComment(buf);
            }
            else if (word == "facet")
            {
                Face face;
                float x, y, z;

                // "normal" を捨てる
                file >> word;
                // 法線座標読み込み
                file >> x >> y >> z;
                face.setNormal(Vec3(x, y, z));
                // "outer", "loop" を捨てる
                file >> word >> word;
                // 面の頂点座標読み込み
                for (int i = 0; i < 3; i++)
                {
                    // "vertex" を捨てる
                    file >> word;
                    // 点座標読み込み
                    file >> x >> y >> z;
                    face.setVertex(Vec3(x, y, z), i);
                }
                // "endloop", "endfacet" を捨てる
                file >> word >> word;
                // 面を登録
                this->faces.push_back(face);
            }
            else if (word == "endsolid")
            {
                break;
            }
        }
    }
    catch (const exception &e)
    {
        // 例外が出たら失敗
        return -1;
    }

    return 0;
}

int Stl::loadBinary(ifstream &file)
{
    try
    {
        // 任意文字列の読み込み
        char s[this->def_comment_byte];
        int size = 0;
        // 任意文字列の読み込み
        file.read(s, this->def_comment_byte);
        this->setComment(s);
        // サイズの読み込み
        file.read(reinterpret_cast<char *>(&size), this->def_size_byte);

        for (int i = 0; i < size; i++)
        {
            Face face;
            float x, y, z;

            file.read(reinterpret_cast<char *>(&x), this->def_value_byte);
            file.read(reinterpret_cast<char *>(&y), this->def_value_byte);
            file.read(reinterpret_cast<char *>(&z), this->def_value_byte);
            face.setNormal(Vec3(x, y, z));
            for (int i = 0; i < 3; i++)
            {
                file.read(reinterpret_cast<char *>(&x), this->def_value_byte);
                file.read(reinterpret_cast<char *>(&y), this->def_value_byte);
                file.read(reinterpret_cast<char *>(&z), this->def_value_byte);
                face.setVertex(Vec3(x, y, z), i);
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
    cout << "comment: " << this->getComment() << endl;
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
        cout << "None" << endl;
        return -1;
    }

    Face face = this->getFace(index);
    Vec3 vec = face.getNormal();

    cout << "normal:(" << vec.getX() << "," << vec.getY() << "," << vec.getZ() << ")" << ends;
    for (int i = 0; i < 3; i++)
    {
        vec = face.getVertex(i);
        cout << "\tp" << i << ":(" << vec.getX() << "," << vec.getY() << "," << vec.getZ() << ")" << ends;
    }
    cout << endl;

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

// getter/setter
vector<Face> Stl::getFace() const
{
    return this->faces;
}

Face Stl::getFace(unsigned int index) const
{
    return this->faces[index];
}

string Stl::getComment() const
{
    return this->comment;
}

void Stl::setComment(string comment)
{
    this->comment = comment;
}

bool Stl::operator==(const Stl &r) const
{
    return this->equals(r);
}

bool Stl::operator!=(const Stl &r) const
{
    return !(*this == r);
}
