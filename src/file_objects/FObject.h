//
// Created by coltenus on 20.03.23.
//

#ifndef LAB6_FOBJECT_H
#define LAB6_FOBJECT_H

#include <filesystem>
#include "../utils/LException.h"

namespace l6 {

    class FObject {
    private:
        std::filesystem::path _path;
        std::string _filename;
        int _level;
        bool _isDirectory;

    public:
        FObject(std::filesystem::path path, int level, bool isDirectory)
        : _path(path), _filename(path.filename().string()), _level(level), _isDirectory(isDirectory) {
            if(!std::filesystem::exists(path))
                throw LException("There is no such file or directory.", 1);
        }
        virtual ~FObject() = default;
        std::filesystem::path GetPathObject() { return _path; }
        std::string GetFileName() { return _filename; }
        int GetLevel() { return _level; }
        virtual void PrintName() { printf("%s\n", _filename.c_str()); }
        std::string GetPath() { return _path.relative_path().string(); }
        std::string GetFullPath() { return _path.parent_path().string() + '/' + _path.filename().string(); }
        bool IsDirectory() { return _isDirectory; }
    };

} // l6

#endif //LAB6_FOBJECT_H
