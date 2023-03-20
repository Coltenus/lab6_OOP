//
// Created by coltenus on 20.03.23.
//

#ifndef LAB6_FILEOBJECT_H
#define LAB6_FILEOBJECT_H

#include <filesystem>

namespace l6 {

    class FileObject {
    private:
        std::filesystem::path _path;
        std::string _filename;

    public:
        FileObject(std::filesystem::path path, std::string filename) : _path(path), _filename(filename) {}
        virtual ~FileObject() = default;
    };

} // l6

#endif //LAB6_FILEOBJECT_H
