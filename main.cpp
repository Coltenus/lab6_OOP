#include <iostream>
#include "src/Filesystem.h"

#define TESTING 1

int main() {
    l6::Filesystem fs;
#if TESTING
    fs.CreateFile("/home/coltenus/vsc/", "test_file.txt", "Hey!\nHi!", true);
    system("cat /home/coltenus/vsc/test_file.txt");
    std::cout << '\n';
    fs.RefreshLua();
    fs.ModifyFile(0, "/home/coltenus/vsc/test_file.txt");
    system("cat /home/coltenus/vsc/test_file.txt");
    std::cout << '\n';
    l6::Filesystem::ModifyFile("/home/coltenus/vsc/test_file.txt", 3);
    system("cat /home/coltenus/vsc/test_file.txt");
    std::cout << '\n';
    fs.Open("/home/coltenus/vsc/");
    fs.Remove("test_file.txt");
    system("cat /home/coltenus/vsc/test_file.txt");
    std::cout << '\n';
    {
        auto buf = fs.GetByName("t*.py");
        std::cout << buf->GetFileName() << '\n';
        delete buf;
    }
    std::cout << '\n';
    {
        auto buf = fs.GetByNameVec("test*.py");
        for (auto &el: buf) {
            std::cout << el->GetFileName() << '\n';
            delete el;
        }
    }
    std::cout << '\n';
    {
        auto buf = fs.GetByName("test?.py");
        std::cout << buf->GetFileName() << '\n';
        delete buf;
    }
    std::cout << '\n';
    {
        auto buf = fs.GetByNameVec("test?.py");
        for (auto &el: buf) {
            std::cout << el->GetFileName() << '\n';
            delete el;
        }
    }
#endif
    return 0;
}

