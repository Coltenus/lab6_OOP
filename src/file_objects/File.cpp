//
// Created by colte on 21.03.2023.
//

#include "File.h"

namespace l6 {
    File::File(std::filesystem::path path, std::string filename, int level) : FObject(path, filename, level) {
        int dotPos = filename.find_last_of('.');
        filename.erase(filename.begin(), filename.begin()+dotPos);
        _extension = filename;
    }

    std::string File::GetExtension(bool clear) {
        std::string result = _extension;

        if(clear) {
            result.erase(result.begin());
        }

        return result;
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