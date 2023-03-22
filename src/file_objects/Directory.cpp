//
// Created by colte on 21.03.2023.
//

#include "Directory.h"
#include "File.h"
#include <unistd.h>
#define UPDATE_TIME 5

namespace l6 {
    Directory::Directory(std::filesystem::path path, int level, bool isMain, bool fetchSubFolders)
            : FObject(path, level, true), _endUpdate(false) {
        if(isMain) {
            _update = new std::thread([this, fetchSubFolders]() {
                while (!_endUpdate) {
                    FetchDir(fetchSubFolders);
                    sleep(UPDATE_TIME);
                }
            });
            _update->detach();
        }
        else {
            _update = nullptr;
            FetchDir(fetchSubFolders);
        }
    }

    Directory::Directory(std::filesystem::path path, bool fetchSubFolders)
            : Directory(path, 0, true, fetchSubFolders) {}

    Directory::Directory(std::filesystem::path path)
            : Directory(path, true) {}

    Directory::Directory(std::filesystem::path path, bool isMain, bool fetchSubFolders)
            : Directory(path, 0, isMain, fetchSubFolders) {}

    Directory::~Directory() {
        if(_update && _update->joinable()) {
            _update->join();
            delete _update;
        }
        ClearFilesData();
    }

    void Directory::FetchDir(bool fetchSubFolders) {
        std::filesystem::path path = GetPathObject();
        ClearFilesData();

        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            if(entry.is_regular_file()) {
                _fileNames.push_back(entry.path().filename().string());
                _files.insert({entry.path().filename().string(),
                               new File(entry.path(), GetLevel() + 1)});
            }
            else if(entry.is_directory())
            {
                _fileNames.push_back(entry.path().filename().string());
                _files.insert({entry.path().filename().string(), new Directory(entry.path(), GetLevel()+1, false, fetchSubFolders)});
                if(fetchSubFolders)
                    dynamic_cast<Directory*>(_files[entry.path().filename().string()])->FetchDir(true);
            }
        }
    }

    void Directory::PrintName() {
        std::string tabs;
        for(int i = 0; i<GetLevel(); i++)
            tabs += "\t";
        printf("%s%s/\n", tabs.c_str(), GetFileName().c_str());
        for(auto& el: _fileNames)
            _files[el]->PrintName();
    }

    FObject *Directory::FindInFiles(std::string filename) {
        FObject* result = nullptr;

        for(std::string& el: _fileNames) {
            if(el == filename) {
                result = _files[el];
                break;
            }
            if(_files[el]->IsDirectory()) {
                auto* buf = dynamic_cast<Directory *>(_files[el]);
                result = buf->FindInFiles(filename);
                if(result)
                    break;
            }
        }

        return result;
    }

    FObject *Directory::FindByPathStack(std::stack<std::string>& path) {
        FObject* result;
        if(path.empty())
            return nullptr;
        std::string buf = path.top();
        path.pop();
        bool contains = false;
        for(auto& el: _fileNames)
            if(el == buf)
                contains = true;
        if(contains) {
            if(!path.empty() && _files[buf]->IsDirectory()) result = dynamic_cast<Directory*>(_files[buf])->FindByPathStack(path);
            else if(path.empty()) return _files[buf];
        } else return nullptr;

        return result;
    }

    FObject *Directory::FindByPath(std::string path) {
        std::stack<std::string> pathStack;
        FObject* result;

        while(path != ".") {
            int slashPos = path.find_last_of('/');
            auto buf = path;
            buf.erase(buf.begin(), buf.begin()+slashPos+1);
            pathStack.push(buf);
            path.erase(path.begin()+slashPos, path.end());
        }

        result = FindByPathStack(pathStack);

        return result;
    }

    void Directory::ClearFilesData() {
        for (auto& el: _fileNames) {
            delete _files[el];
        }
        _fileNames.clear();
        _files.clear();
    }

    bool Directory::HasName(std::string name) const {
        bool result = false;

        for(const std::string& el: _fileNames)
            if(el == name) {
                result = true;
                break;
            }

        return result;
    }
} // l6