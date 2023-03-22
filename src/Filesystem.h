//
// Created by coltenus on 22.03.23.
//

#ifndef LAB6_FILESYSTEM_H
#define LAB6_FILESYSTEM_H

#include "file_objects/file_objects.h"

namespace l6 {

    class Filesystem {
    private:
        FObject* _current;

    public:
        Filesystem();
        explicit Filesystem(std::string path);
        ~Filesystem();
        void Print();
        void Open(std::string path);
        void Open(std::string path, bool inCurrent);
        void Clear();
        bool IsOpened();
        void GoUpper();
        void CreateFolder(std::string name, bool rewrite = false);
        void CreateFile(std::string name, bool rewrite = false);
        void Remove(std::string name);
        void Rename(std::string oldName, std::string newName);
        void Replace(std::string name, std::string path);
        void Copy(std::string name, std::string path, std::string newName);
        std::string GetCurrentPath();
    };

} // l6

#endif //LAB6_FILESYSTEM_H