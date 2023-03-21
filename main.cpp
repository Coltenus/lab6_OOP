#include <iostream>
#include "src/file_objects/Directory.h"

int main() {
    l6::FObject* cur = new l6::Directory(std::filesystem::path("."), "current", 0);
    cur->PrintName();
    delete cur;
    return 0;
}

