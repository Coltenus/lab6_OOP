//
// Created by coltenus on 22.03.23.
//

#include "Filesystem.h"
#include <unistd.h>
#define DELAY_FOR_FETCH 10000
#ifdef _WIN32
#define ROOT "C:\\"
#define UP ":\\"
#endif
#ifdef __linux__
#define ROOT "/"
#define UP ROOT
#endif

namespace l6 {
    Filesystem::Filesystem() : _current(nullptr) {
        Open(ROOT);
    }

    Filesystem::Filesystem(std::string path) : _current(nullptr) {
        Open(path);
    }

    Filesystem::~Filesystem() {
        Clear();
    }

    void Filesystem::Print() {
        if(_current) {
            _current->PrintName();
            printf("\n");
        }
    }

    void Filesystem::Open(std::string path) {
        Clear();
        _current = OpenObject(path);
    }

    void Filesystem::Clear() {
        if(_current) {
            delete _current;
            _current = nullptr;
        }
    }

    void Filesystem::Open(std::string path, bool inCurrent) {
        if(inCurrent) {
            std::string result;
            if(_current && _current->IsDirectory()) {
                result = dynamic_cast<Directory*>(_current)->FindByPath(path);
            }
            if(std::filesystem::is_directory(result)) {
                Open(result);
            }
        }
        else Open(path);
    }

    bool Filesystem::IsOpened() {
        return _current;
    }

    void Filesystem::GoUpper() {
        if(_current && !_current->GetFullPath().ends_with(UP)) {
            std::string newPath = _current->GetPathObject().parent_path().string();
            Open(newPath);
        }
    }

    void Filesystem::CreateFolder(std::string name, bool rewrite) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if (current->HasName(name) && rewrite)
                std::filesystem::remove_all(current->GetFullPath() + '/' + name);
            if (rewrite || !current->HasName(name))
                std::filesystem::create_directory(current->GetFullPath() + '/' + name);
            current->FetchDir();
        }
    }

    void Filesystem::CreateFile(std::string name, bool rewrite) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if (current->HasName(name) && rewrite)
                std::filesystem::remove_all(current->GetFullPath() + '/' + name);
            if (rewrite || !current->HasName(name))
                std::filesystem::create_directory(current->GetFullPath() + '/' + name);
            current->FetchDir();
        }
    }

    void Filesystem::Remove(std::string name) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(name))
                std::filesystem::remove_all(current->GetFullPath() + '/' + name);
            current->FetchDir();
        }
    }

    void Filesystem::Rename(std::string oldName, std::string newName) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(oldName) && !current->HasName(newName))
                std::filesystem::rename(current->GetFullPath() + '/' + oldName, current->GetFullPath() + '/' + newName);
            current->FetchDir();
        }
    }

    void Filesystem::Replace(std::string name, std::string path) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(name) && std::filesystem::exists(path))
                std::filesystem::rename(current->GetFullPath() + '/' + name, path + '/' + name);
            current->FetchDir();
        }
    }

    void Filesystem::Copy(std::string name, std::string path, std::string newName) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(name) && std::filesystem::exists(path))
                std::filesystem::copy(current->GetFullPath() + '/' + name, path + '/' + newName, std::filesystem::copy_options::recursive);
            current->FetchDir();
        }
    }

    std::string Filesystem::GetCurrentPath() const {
        if(_current)
            return _current->GetFullPath();
        else return "";
    }

    FObject *Filesystem::GetByPath(std::string path) {
        FObject* result = nullptr;
        if(_current) {
            if(path.ends_with('/') || path.ends_with('\\'))
                path.erase(path.end()-1);
            if(!_current->IsDirectory()) {
                result = GetNameIfCurrent(path);
            } else {
                Directory* dir = dynamic_cast<Directory *>(_current);
                std::string fullPath = dir->FindByPath(path);
                if(!fullPath.empty()) {
                    result = OpenObject(fullPath);
                }
            }
        }

        return result;
    }

    FObject* Filesystem::GetNameIfCurrent(std::string path) {
        FObject* result = nullptr;
        if(path.ends_with(_current->GetFileName()))
            result = _current;
        return result;
    }

    FObject *Filesystem::OpenObject(std::string path) {
        FObject* result = nullptr;

        try {
            if (std::filesystem::is_directory(path)) {
                result = new Directory(std::filesystem::path(path));
            } else if (std::filesystem::is_regular_file(path)) {
                result = new File(std::filesystem::path(path));
            }
            if(result && result->IsDirectory()) {
                Directory* dir = dynamic_cast<Directory *>(result);
                dir->FetchDir();
            }
        }
        catch (LException& ex) {
            printf("%s\n", ex.what());
            result = nullptr;
        }

        return result;
    }

    FObject *Filesystem::GetByName(std::string filename) {
        FObject* result = nullptr;
        if(_current) {
            if(!_current->IsDirectory() && _current->GetFileName() == filename) {
                result = _current;
            } else {
                Directory* dir = dynamic_cast<Directory *>(_current);
                std::string fullPath = dir->FindInFiles(filename);
                if(!fullPath.empty()) {
                    result = OpenObject(fullPath);
                }
            }
        }

        return result;
    }

    Directory *Filesystem::GetRoot() {
        return new Directory(ROOT);
    }
} // l6