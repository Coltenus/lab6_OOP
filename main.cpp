#include <dirent.h>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void OpenDirectory(const char*);

int main() {
    OpenDirectory(".");
    return 0;
}

void OpenDirectory(const char* _path) {
    fs::path path = _path;
    for (const auto & entry : fs::directory_iterator(path)) {
        if(entry.is_regular_file())
            std::cout << entry.path().relative_path() << std::endl;
        else if(entry.is_directory())
        {
            OpenDirectory(entry.path().generic_string().c_str());
        }
    }
}

