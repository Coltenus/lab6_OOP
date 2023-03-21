//
// Created by coltenus on 20.03.23.
//

#ifndef LAB6_FOBJECT_H
#define LAB6_FOBJECT_H

#include <filesystem>

namespace l6 {

    class FObject {
    private:
        std::filesystem::path _path;
        std::string _filename;
        bool _existing;
        int _level;

    public:
        FObject(std::filesystem::path path, std::string filename, int level)
        : _path(path), _filename(filename), _existing(true), _level(level) {}
        virtual ~FObject() = default;
        std::filesystem::path GetPath() { return _path; }
        std::string GetFileName() { return _filename; }
        int GetLevel() { return _level; }
        bool IsExisting() { return _existing; }
        void Missed() { _existing = false; }
        virtual void PrintName() { printf("%s\n", _filename.c_str()); }
    };

} // l6

#endif //LAB6_FOBJECT_H
