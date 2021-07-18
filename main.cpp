#include <iostream>
#include "obj.hpp"
#include "optarg.h"

#define OPTSIZE 10

#define VERSION "1.0.0"
#define ISSUE_LINK "https://github.com/GrapeJuicer/StlChecker/issues"

using namespace std;
using namespace stlset;

void printHelp();
void printVersion();

bool isShow = true;

int main(int argc, char *argv[])
{
    /*
    help
    version
    format
    result
    permission
    */
    char shortotps[] = "hvf:rp:u:";
    struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"format", required_argument, NULL, 'f'},
        {"result", no_argument, NULL, 'r'},
        {"permission", required_argument, NULL, 'p'},
        {"rule", required_argument, NULL, 'u'},
        OPT_END};
    struct optarg findopts[OPTSIZE];
    int folen;
    string srctype = "bb";
    double permission_range;
    bool set_permission = false;
    string opstr;
    int rule = rule::direction;

    bool result;

    Stl s[2];

    // 引数処理
    folen = optprocess(argc, argv, shortotps, longopts, findopts, OPTSIZE);

    for (int i = 0; i < folen; i++)
    {
        switch (findopts[i].opt)
        {
        case 'h':
            printHelp();
            return 0;
        case 'v':
            printVersion();
            return 0;
        case 'f':
            srctype = findopts[i].arg;
            break;
        case 'r':
            isShow = false;
            break;
        case 'p':
            set_permission = true;
            permission_range = atof(findopts[i].arg);
            break;
        case 'u':
            opstr = findopts[i].arg;
            if (opstr == "0" || opstr == "direction")
            {
                rule = rule::direction;
            }
            else if (opstr == "1" || opstr == "point")
            {
                rule = rule::point;
            }
            else
            {
                cout << "Error: Invalid option value: " << opstr << endl;
                return -1;
            }
            break;
        default:
            break;
        }
    }

    if (argc - optind != 2) // file1 file2
    {
        cerr << "Error: Invlid argumanets." << endl;
        return -1;
    }

    if (set_permission && permission_range <= 0)
    {
        cerr << "Error: option.permission: Invalid value." << endl;
        return -1;
    }

    for (int i = 0; i < 2; i++)
    {
        if (s[i].load(argv[optind + i], srctype[i] == 'b'))
        {
            cerr << "Error: Failed to open a file: " << argv[optind + i] << endl;
            return -1;
        }
    }

    // check
    if (set_permission)
    {
        result = s[0].inRangeWithShow(s[1], permission_range, isShow, rule);
    }
    else
    {
        result = s[0].equalsWithShow(s[1], isShow, level::exactly);
    }
    
    // show result
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

void printHelp()
{
    cout << "Usage: stlc [options...] file1 file2" << endl;
    cout << "Options:" << endl;
    cout << "  -h          , --help                 Display this information." << endl;
    cout << "  -v          , --version              Display command version information." << endl;
    cout << "  -f<b/t...>  , --format=<b/t...> ," << endl;
    cout << "  -f <b/t...> , --format <b/t...>      Specify the format of the file." << endl;
    cout << "                                       Example: --format tb" << endl;
    cout << "                                       It means file1->text, file2->binary." << endl;
    cout << "  -r          , --result               Display only if there is a match. It returns \'True\' or \'False\'" << endl;
    cout << endl;
    cout << "For bug reporting instructions, please see:" << endl;
    cout << "<" << ISSUE_LINK << ">." << endl;
}

void printVersion()
{
    cout << "stlc " << VERSION << endl;
    cout << "Copyright (C) 2021 GrapeJuice." << endl;
}
