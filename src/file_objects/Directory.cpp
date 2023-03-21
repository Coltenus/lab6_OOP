//
// Created by colte on 21.03.2023.
//

#include "Directory.h"
#include "File.h"

namespace l6 {
    Directory::Directory(std::filesystem::path path, std::string filename, int level) : FObject(path, filename, level) {
        FetchDir();
    }

    Directory::~Directory() {
        for (FObject* el: files) {
            delete el;
        }
    }

    void Directory::FetchDir() {
        std::filesystem::path path = GetPath();
        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            if(entry.is_regular_file())
                files.insert(new File(entry.path(), entry.path().filename().string(), GetLevel()+1));
            else if(entry.is_directory())
            {
                files.insert(new Directory(entry.path(), entry.path().filename().string(), GetLevel()+1));
            }
        }
    }

    void Directory::PrintName() {
        std::string tabs = "";
        for(int i = 0; i<GetLevel(); i++)
            tabs += "\t";
        printf("%s%s/\n", tabs.c_str(), GetFileName().c_str());
        for(auto& el: files)
            el->PrintName();
    }
} // l6