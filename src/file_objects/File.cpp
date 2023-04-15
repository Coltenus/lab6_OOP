//
// Created by colte on 21.03.2023.
//

#include "File.h"
#include <utility>

namespace l6 {
    File::File(const std::filesystem::path& path, int level) : FObject(path, level, false) {
        std::string filename = GetFileName();
        int dotPos = filename.find_last_of('.');
        if(dotPos != -1) {
            filename.erase(filename.begin(), filename.begin()+dotPos+1);
            _extension = filename;
        }
        else _extension = "no ext";
        {
            tm *clock;
            struct stat attrib{};
            stat(absolute(path).c_str(), &attrib);
            clock = localtime(&(attrib.st_mtime));
            _modDate = {.year = clock->tm_year+1900, .month = clock->tm_mon + 1, .day = clock->tm_mday,
                    .hour = clock->tm_hour, .minute = clock->tm_min};
        }
        _size = std::filesystem::file_size(path);
    }

    File::File(const std::filesystem::path& path)
    : File(path, 0) {}

    std::string File::GetExtension() const {
        return _extension;
    }

    void File::PrintName(bool check) {
        if(check && GetLevel() < 2){
            std::string tabs;
            for (int i = 0; i < GetLevel(); i++)
                tabs += "\t";
            std::cout << tabs << GetFileName() << '(' << GetExtension() << ") creation date:" << GetCreationDate() \
 << ", mod date:" << GetModificationDate() << ", size:" << GetSize() << " bytes\n";
        }
    }

    FObject::Date File::GetModificationDate() const {
        return _modDate;
    }

    std::uint64_t File::GetSize() {
        return _size;
    }
} // l6