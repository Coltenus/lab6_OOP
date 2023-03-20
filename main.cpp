#include <dirent.h>
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void OpenDirectory(const char*);
void OpenDirectoryN(const char*);

int main() {
    OpenDirectory(".");
    std::cout << "\n\n\n";
    OpenDirectoryN(".");
    return 0;
}

void OpenDirectory(const char* _path) {
    std::string path(_path);
    path += "/";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                printf ("%s%s\n", path.c_str(), ent->d_name);
            }
            else if(ent->d_type == DT_DIR && std::string(ent->d_name) != ".." && std::string(ent->d_name) != ".")
            {
                OpenDirectory((path + ent->d_name).c_str());
            }
        }
        closedir (dir);
    }
}

void OpenDirectoryN(const char* _path) {
    fs::path path = _path;
    for (const auto & entry : fs::directory_iterator(path)) {
        if(entry.is_regular_file())
            std::cout << entry.path().relative_path().c_str() << std::endl;
        else if(entry.is_directory())
        {
            OpenDirectoryN((entry.path()).c_str());
        }
    }
}

