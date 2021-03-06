#include <iostream>
#include <string>
#include "obj.hpp"
#include "optarg.h"

// オプションの種類の上限(現在は6)
#define OPTSIZE 10
// バージョン
#define VERSION "1.1.0"
// リンク
#define ISSUE_LINK "https://github.com/GrapeJuicer/StlChecker/issues"

// 名前空間
using namespace std;
using namespace stlset;

// プロトタイプ宣言
void printHelp();
void printVersion();

// グローバル変数
bool isShow = true;

int main(int argc, char *argv[])
{
    /*
    h, help      ... ヘルプを表示
    v, version   ... バージョンを表示
    f, format    ... ファイルのフォーマットを指定
    r, result    ... 結果(True/False)のみ表示
    t, tolerance ... 許容誤差を設定
    u, rule      ... 許容誤差設定時の誤差計算方式 (0,component:点の各成分の距離 / 1,vertex:点同士の距離)
    */

    // オプション(短)
    char shortotps[] = "hvf:rt:u:";
    // オプション(長)
    struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"format", required_argument, NULL, 'f'},
        {"result", no_argument, NULL, 'r'},
        {"tolerance", required_argument, NULL, 't'},
        {"rule", required_argument, NULL, 'u'},
        OPT_END};
    // 指定されたオプションの情報を格納する配列
    struct optarg findopts[OPTSIZE];
    // 指定されたオプションの数
    int folen;
    // ファイルのフォーマット (b:バイナリ / t:テキスト)
    string srctype = "bb";
    // 許容範囲
    double tolerance_range;
    // 許容範囲設定フラグ
    bool set_tolerance = false;
    // 引数文字列処理用
    string opstr;
    // 計算方式
    int rule = rule::component;
    // 比較結果
    bool result;
    // 引数で与えられた2つのSTLファイルのデータ
    Stl s[2];

    // 引数処理
    folen = optprocess(argc, argv, shortotps, longopts, findopts, OPTSIZE);

    // 指定されたオプションの処理
    for (int i = 0; i < folen; i++)
    {
        switch (findopts[i].opt)
        {
        case 'h': // help
            printHelp();
            return 0;
        case 'v': // version
            printVersion();
            return 0;
        case 'f': // format
            srctype = findopts[i].arg;
            break;
        case 'r': // result
            isShow = false;
            break;
        case 't': // tolerance
            set_tolerance = true;
            tolerance_range = atof(findopts[i].arg);
            // 許容誤差が設定されているが，許容範囲が不正な場合，エラーを出力して終了
            if (tolerance_range < 0)
            {
                cerr << "Error: option.tolerance: Invalid value: " << tolerance_range << endl;
                return -1;
            }
            break;
        case 'u': // rule
            opstr = findopts[i].arg;
            if (opstr == to_string(rule::component) || opstr == "c" || opstr == "component")
            {
                rule = rule::component;
            }
            else if (opstr == to_string(rule::vertex) || opstr == "v" || opstr == "vertex")
            {
                rule = rule::vertex;
            }
            else
            {
                cerr << "Error: option.rule: Invalid value: " << opstr << endl;
                return -1;
            }
            break;
        default:
            break;
        }
    }

    // オプションを含めない引数の個数が2つで無い場合，エラーを出力して終了
    if (argc - optind != 2) // file1 file2
    {
        cerr << "Error: Invalid argumanets." << endl;
        return -1;
    }

    // 引数で与えられた2つのファイルを読み込む
    for (int i = 0; i < 2; i++)
    {
        if (s[i].load(argv[optind + i], srctype[i] == 'b'))
        {
            cerr << "Error: Failed to open a file: " << argv[optind + i] << endl;
            return -1;
        }
    }

    // ファイル内容の比較
    if (set_tolerance)
    {
        result = s[0].inRangeWithShow(s[1], tolerance_range, isShow, rule);
    }
    else
    {
        result = s[0].equalsWithShow(s[1], isShow, level::exactly);
    }

    // 結果の表示
    if (isShow)
    {
        if (result)
        {
            cout << "[O] file structure matched." << endl;
        }
        else
        {
            cout << "[X] file structure unmatched." << endl;
        }
    }
    else
    {
        cout << (result ? "True" : "False") << endl;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////
// overview:
//      printHelp は，プログラムのヘルプを出力する関数である．
// args:
//      なし
// return:
//      なし
////////////////////////////////////////////////////////////////////////
void printHelp()
{
    cout << "Usage: stlc [options...] file1 file2" << endl;
    cout << "Options:" << endl;
    cout << "  -h          , --help                 Display this information." << endl;
    cout << "  -v          , --version              Display command version information." << endl;
    cout << "  -f<b/t...>  , --format=<b/t...> ," << endl;
    cout << "  -f <b/t...> , --format <b/t...>      Specify the format of the file. Default is binary." << endl;
    cout << "                                       Example: --format tb" << endl;
    cout << "                                       It means file1 -> text, file2 -> binary." << endl;
    cout << "  -r          , --result               Display only if there is a match. It returns \'True\' or \'False\'" << endl;
    cout << "  -t<value>   , --tolerance=<value> ," << endl;
    cout << "  -t <value>  , --tolerance <value>    Set the tolerance." << endl;
    cout << "                                       Note: Tolerance deviations may occur due to calculation errors." << endl;
    cout << "  -u<rule>    , --rule=<rule> ," << endl;
    cout << "  -u <rule>   , --rule <rule>          Set the calculation method when the tolerance is set." << endl;
    cout << "                                       The following arguments can be given to <rule>." << endl;
    cout << "                                           " << rule::component << ", c, component ... Calculate the distance for each component of a vertex" << endl;
    cout << "                                           " << rule::vertex << ", v, vertex    ... Calculate by the distance between two vertexes" << endl;
    cout << endl;
    cout << "For bug reporting instructions, please see:" << endl;
    cout << "<" << ISSUE_LINK << ">." << endl;
}

////////////////////////////////////////////////////////////////////////
// overview:
//      printVersion は，プログラムのバージョンを出力する関数である．
// args:
//      なし
// return:
//      なし
////////////////////////////////////////////////////////////////////////
void printVersion()
{
    cout << "stlc " << VERSION << endl;
    cout << "Copyright (C) 2021 GrapeJuice." << endl;
}
