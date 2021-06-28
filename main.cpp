#include <iostream>
#include "obj.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4) // .exe file1 file2 source_type
    {
        cout << "Error: Invlid argumanets." << endl;
        return -1;
    }

    string srctype = argv[argc - 1];

    Stl s[2];

    for (int i = 1; i < argc - 1; i++)
    {
        // cout << "File: " << argv[i] << " -----" << endl;

        if (s[i - 1].load(argv[i], srctype[i - 1] == 'b'))
        {
            cout << "Error: Failed to open a file: " << argv[i] << endl;
            return -1;
        }

        // s[i - 1].show();
    }

    if (s[0].equalsWithShow(s[1], true))
    {
        cout << "[O] file structure matched." << endl;
    }
    else
    {
        cout << "[X] file structure unmatched." << endl;
    }
    
    return 0;
}
