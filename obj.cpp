#include "obj.hpp"

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

float Vec3::length()
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
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

/////////////////////////////////////////////////////////////////////////////////////////////////

Stl::Stl()
{
}

Stl::~Stl()
{
}

int Stl::size()
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

                writen_face_size++; // 面の数を1増やす(テキスト形式では定義数と実際の数は必ず同じ)
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
        file.read(s, this->def_comment_byte);
        this->comment = s;

        // サイズの読み込み
        file.read(reinterpret_cast<char *>(&this->writen_face_size), this->def_size_byte);

        // while (!file.eof())
        for (int i = 0; i < this->writen_face_size; i++)
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
