//
// Created by colte on 21.03.2023.
//

#include "Directory.h"
#include "File.h"
#include <unistd.h>

#include <utility>
#define UPDATE_TIME 5

namespace l6 {
    Directory::Directory(std::filesystem::path path, int level)
            : FObject(std::move(path), level, true) {}

    Directory::Directory(std::filesystem::path path)
            : Directory(std::move(path), 0) {}

    Directory::~Directory() {
        ClearFilesData();
    }

    void Directory::FetchDir() {
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
                _files.insert({entry.path().filename().string(), new Directory(entry.path(), GetLevel()+1)});
            }
        }
    }

    void Directory::PrintName() {
        std::string tabs;
        for(int i = 0; i<GetLevel(); i++)
            tabs += "\t";
        printf("%s%s/\n", tabs.c_str(), GetFileName().c_str());
        FetchDir();
        for(auto& el: _fileNames)
            _files[el]->PrintName();
        if(GetLevel() == 0)
            ClearFilesData();
    }

    std::string Directory::FindInFiles(const std::string& filename) {
        std::string result;
        FetchDir();

        for(std::string& el: _fileNames) {
            if(el == filename) {
                result = _files[el]->GetFullPath();
                break;
            }
            else if(_files[el]->IsDirectory()) {
                auto* buf = dynamic_cast<Directory *>(_files[el]);
                result = buf->FindInFiles(filename);
                if(!result.empty())
                    break;
            }
        }
        if(GetLevel() == 0) {
            ClearFilesData();
        }

        return result;
    }

    std::string Directory::FindByPathStack(std::stack<std::string>& path) {
        std::string result;
        if(path.empty())
            return "";
        FetchDir();
        std::string buf = path.top();
        path.pop();
        bool contains = false;
        for(auto& el: _fileNames)
            if(el == buf)
                contains = true;
        if(contains) {
            if(!path.empty() && _files[buf]->IsDirectory()) result = dynamic_cast<Directory*>(_files[buf])->FindByPathStack(path);
            else if(path.empty()) result = _files[buf]->GetFullPath();
        }
        if(GetLevel() == 0) {
            ClearFilesData();
        }

        return result;
    }

    std::string Directory::FindByPath(std::string path) {
        std::stack<std::string> pathStack;
        std::string result;

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

    bool Directory::HasName(const std::string& name) const {
        bool result = false;

        for(const std::string& el: _fileNames)
            if(el == name) {
                result = true;
                break;
            }

        return result;
    }

    std::string Directory::FindInFiles(const std::string& start, const std::string& end, bool greater) {
        std::string result;
        int size = start.size() + end.size() + 1;
        FetchDir();

        for(std::string& el: _fileNames) {
            if(el.starts_with(start) && el.ends_with(end)
            && (greater && el.size() >= size || !greater && el.size() == size)) {
                result = _files[el]->GetFullPath();
                break;
            }
            else if(_files[el]->IsDirectory()) {
                auto* buf = dynamic_cast<Directory *>(_files[el]);
                result = buf->FindInFiles(start, end, greater);
                if(!result.empty())
                    break;
            }
        }
        if(GetLevel() == 0) {
            ClearFilesData();
        }

        return result;
    }

    void Directory::FindInFilesVec(std::vector<std::string>& files, const std::string &filename) {
        FetchDir();

        for(std::string& el: _fileNames) {
            if(el == filename) {
                files.push_back(_files[el]->GetFullPath());
            }
            else if(_files[el]->IsDirectory()) {
                auto* buf = dynamic_cast<Directory *>(_files[el]);
                buf->FindInFilesVec(files, filename);
            }
        }
        if(GetLevel() == 0) {
            ClearFilesData();
        }
    }

    void Directory::FindInFilesVec(std::vector<std::string> &files, const std::string &start,
                                   const std::string &end, bool greater) {
        int size = start.size() + end.size() + 1;
        FetchDir();

        for(std::string& el: _fileNames) {
            if(el.starts_with(start) && el.ends_with(end)
               && (greater && el.size() >= size || !greater && el.size() == size)) {
                files.push_back(_files[el]->GetFullPath());
            }
            else if(_files[el]->IsDirectory()) {
                auto* buf = dynamic_cast<Directory *>(_files[el]);
                buf->FindInFilesVec(files, start, end, greater);
            }
        }
        if(GetLevel() == 0) {
            ClearFilesData();
        }
    }
} // l6