//
// Created by colte on 21.03.2023.
//

#ifndef LAB6_DIRECTORY_H
#define LAB6_DIRECTORY_H

#include <unordered_map>
#include <vector>
#include <stack>
#include <thread>
#include "FObject.h"

namespace l6 {

    class Directory : public FObject {
    private:
        std::thread* _update;
        std::unordered_map<std::string, FObject*> _files;
        std::vector<std::string> _fileNames;
        FObject* FindByPathStack(std::stack<std::string>& path);
        bool _endUpdate;

    public:
        Directory(std::filesystem::path path, int level, bool isMain, bool fetchSubFolders);
        Directory(std::filesystem::path path, bool fetchSubFolders);
        Directory(std::filesystem::path path);
        Directory(std::filesystem::path path, bool isMain, bool fetchSubFolders);
        ~Directory() override;
        void FetchDir(bool fetchSubFolders);
        void PrintName() override;
        FObject* FindInFiles(std::string filename);
        FObject* FindByPath(std::string path);
        void ClearFilesData();
        bool HasName(std::string name);
    };

} // l6

#endif //LAB6_DIRECTORY_H
