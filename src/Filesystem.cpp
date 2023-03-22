//
// Created by coltenus on 22.03.23.
//

#include "Filesystem.h"
#include <unistd.h>
#define DELAY_FOR_FETCH 10000
#ifdef _WIN32
#define ROOT ":/"
#endif
#ifdef __linux__
#define ROOT "home/"
#endif

namespace l6 {
    Filesystem::Filesystem() : _current(nullptr) {}

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
        try {
            if (std::filesystem::is_directory(path)) {
                _current = new Directory(std::filesystem::path(path));
            } else if (std::filesystem::is_regular_file(path)) {
                _current = new File(std::filesystem::path(path));
            }
            if(_current)
                usleep(DELAY_FOR_FETCH);
        }
        catch (LException& ex) {
            printf("%s\n", ex.what());
            _current = nullptr;
        }
    }

    void Filesystem::Clear() {
        if(_current) {
            delete _current;
            _current = nullptr;
        }
    }

    void Filesystem::Open(std::string path, bool inCurrent) {
        if(inCurrent) {
            FObject* result = nullptr;
            if(_current && _current->IsDirectory()) {
                result = dynamic_cast<Directory*>(_current)->FindByPath(path);
            }
            if(result && result->IsDirectory()) {
                std::string newPath = result->GetFullPath();
                Open(newPath);
            }
        }
        else Open(path);
    }

    bool Filesystem::IsOpened() {
        return _current;
    }

    void Filesystem::GoUpper() {
        if(!_current->GetFullPath().ends_with(ROOT)) {
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
            current->FetchDir(true);
        }
    }

    void Filesystem::CreateFile(std::string name, bool rewrite) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if (current->HasName(name) && rewrite)
                std::filesystem::remove_all(current->GetFullPath() + '/' + name);
            if (rewrite || !current->HasName(name))
                std::filesystem::create_directory(current->GetFullPath() + '/' + name);
            current->FetchDir(true);
        }
    }

    void Filesystem::Remove(std::string name) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(name))
                std::filesystem::remove_all(current->GetFullPath() + '/' + name);
            current->FetchDir(true);
        }
    }

    void Filesystem::Rename(std::string oldName, std::string newName) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(oldName) && !current->HasName(newName))
                std::filesystem::rename(current->GetFullPath() + '/' + oldName, current->GetFullPath() + '/' + newName);
            current->FetchDir(true);
        }
    }

    void Filesystem::Replace(std::string name, std::string path) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(name) && std::filesystem::exists(path))
                std::filesystem::rename(current->GetFullPath() + '/' + name, path + '/' + name);
            current->FetchDir(true);
        }
    }

    void Filesystem::Copy(std::string name, std::string path, std::string newName) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(name) && std::filesystem::exists(path))
                std::filesystem::copy(current->GetFullPath() + '/' + name, path + '/' + newName, std::filesystem::copy_options::recursive);
            current->FetchDir(true);
        }
    }

    std::string Filesystem::GetCurrentPath() const {
        return _current->GetFullPath();
    }
} // l6