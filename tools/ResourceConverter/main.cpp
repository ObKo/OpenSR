#include "pkg2rpkg.h"
#include <iostream>

void showHelp()
{
    std::cout << "Usage: rangersresconv command [arguments]" << std::endl;
    std::cout << "Commands: " << std::endl;
    std::cout << "\tpkg2rpkg pkg_file rpkg_file - convert pkg files from original game to rpkg format." << std::endl;
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        showHelp();
        return 0;
    }
    if(std::string(argv[1]) == "pkg2rpkg")
    {
        if(argc < 4)
        {
            showHelp();
            return 0;
        }
        Rangers::pkg2rpkg(argv[2], argv[3]);
    }
}
