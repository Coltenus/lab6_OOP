//
// Created by colte on 21.03.2023.
//

#include "File.h"

namespace l6 {
    File::File(std::filesystem::path path, int level) : FObject(path, level, false) {
        std::string filename = GetFileName();
        int dotPos = filename.find_last_of('.');
        if(dotPos != -1) {
            filename.erase(filename.begin(), filename.begin()+dotPos+1);
            _extension = filename;
        }
        else _extension = "no ext";
    }

    File::File(std::filesystem::path path)
    : File(path, 0) {}

    std::string File::GetExtension() {
        return _extension;
    }

    std::string File::GetNameOnly() {
        std::string result = GetFileName();
        int dotPos = result.find_last_of('.');
        result.erase(result.begin()+dotPos, result.end());
        return result;
    }

    void File::PrintName() {
        std::string tabs = "";
        for(int i = 0; i<GetLevel(); i++)
            tabs += "\t";
        printf("%s%s(%s)\n", tabs.c_str(), GetNameOnly().c_str(), GetExtension().c_str());
    }
} // l6