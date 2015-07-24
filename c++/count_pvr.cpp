#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <vector>

using namespace std;

class Task
{

};

int do_main(int argc, char **argv)
{
    int opt_res = 0;
    bool is_file = false;
    int opt_flag = 0;
    vector<char *> files;
    while ((opt_res = getopt(argc, argv, "f:")) != EOF)
    {
        switch (opt_res)
        {
            case 'f':
                {
                    is_file = true;
                    opt_flag = optind;
                    files.push_back(optarg);
                }
                break;
            default:
                break;

        }
    }

    if (is_file) 
    {
        while (opt_flag < argc)
        {
            files.push_back(argv[opt_flag]);
            opt_flag++;
        }
    }

}

void print_files(const vector<char *> & files)
{
    vector<char *>::iterator  iter;
    for (iter =  g_files.begin(); iter !=  g_files.end(); ++iter)
    {
        printf("%s\n", *iter);
    }
}

int main(int argc, char *argv[])
{
    do_main(argc, argv);    
    return 0;
}
