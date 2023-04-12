//
// Created by coltenus on 22.03.23.
//

#include "Filesystem.h"
#include <unistd.h>
#include <fstream>
#include <utility>

namespace l6 {
    Filesystem::Filesystem() : _current(nullptr) {
        Open(ROOT);
    }

    Filesystem::Filesystem(std::string path) : _current(nullptr) {
        Open(std::move(path));
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
        _current = OpenObject(std::move(path));
    }

    void Filesystem::Clear() {
        if(_current) {
            delete _current;
            _current = nullptr;
        }
    }

    void Filesystem::Open(const std::string& path, bool inCurrent) {
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

    void Filesystem::CreateFolder(const std::string& name, bool rewrite) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if (current->HasName(name) && rewrite)
                std::filesystem::remove_all(current->GetFullPath() + '/' + name);
            if (rewrite || !current->HasName(name))
                std::filesystem::create_directory(current->GetFullPath() + '/' + name);
            current->FetchDir();
        }
    }

    void Filesystem::Remove(const std::string& name) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(name))
                std::filesystem::remove_all(current->GetFullPath() + '/' + name);
            current->FetchDir();
        }
    }

    void Filesystem::Rename(const std::string& oldName, const std::string& newName) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(oldName) && !current->HasName(newName))
                std::filesystem::rename(current->GetFullPath() + '/' + oldName, current->GetFullPath() + '/' + newName);
            current->FetchDir();
        }
    }

    void Filesystem::Replace(const std::string& name, const std::filesystem::path& path) {
        if(_current && _current->IsDirectory()) {
            auto *current = dynamic_cast<Directory*>(_current);
            if(current->HasName(name) && std::filesystem::exists(path))
                std::filesystem::rename(current->GetFullPath() + '/' + name, path.string() + '/' + name);
            current->FetchDir();
        }
    }

    void Filesystem::Copy(const std::string& name, const std::string& path, const std::string& newName) {
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

    FObject* Filesystem::GetNameIfCurrent(const std::string& path) {
        FObject* result = nullptr;
        if(path.ends_with(_current->GetFileName()))
            result = _current;
        return result;
    }

    FObject *Filesystem::OpenObject(const std::string& path) {
        FObject* result = nullptr;

        try {
            if (std::filesystem::is_directory(path)) {
                result = new Directory(std::filesystem::path(path));
            } else if (std::filesystem::is_regular_file(path)) {
                result = new File(std::filesystem::path(path));
            }
            if(result && result->IsDirectory()) {
                auto* dir = dynamic_cast<Directory *>(result);
                dir->FetchDir();
            }
        }
        catch (LException& ex) {
            printf("%s\n", ex.what());
            result = nullptr;
        }

        return result;
    }

    FObject *Filesystem::GetByName(const std::string& filename) {
        FObject* result = nullptr;
        if(_current) {
            if(!_current->IsDirectory() && _current->GetFileName() == filename) {
                result = _current;
            } else {
                auto* dir = dynamic_cast<Directory *>(_current);
                std::string fullPath;
                int fMaskS = filename.find('*');
                int fMaskQ = filename.find('?');
                if(fMaskS != -1 && fMaskS == filename.find_last_of('*')) {
                    std::string start = filename.substr(0, fMaskS);
                    std::string end = filename.substr(fMaskS+1, filename.size()-1);
                    fullPath = dir->FindInFiles(start, end, true);
                }
                else if(fMaskQ != -1 && fMaskQ == filename.find_last_of('?')) {
                    std::string start = filename.substr(0, fMaskQ);
                    std::string end = filename.substr(fMaskQ+1, filename.size()-1);
                    fullPath = dir->FindInFiles(start, end, false);
                }
                else fullPath = dir->FindInFiles(filename);
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

    int Filesystem::CreateFile(const std::filesystem::path& path, std::string filename, std::string data, bool rewrite) {
        return CreateFile(path, std::move(filename), data.size(), data.c_str(), rewrite);
    }

    int Filesystem::ModifyFile(const std::filesystem::path& path, std::uint32_t size) {
        if(!std::filesystem::is_regular_file(path))
            return 1;
        if(std::filesystem::file_size(path) <= size)
            return 2;
        char* data = new char[size];
        {
            std::ifstream ifs(path, std::ios::in);
            ifs.read(data, size);
        }
        {
            std::ofstream ofs(path, std::ios::trunc | std::ios::out);
            ofs.write(data, size);
        }
        delete[] data;
        return 0;
    }

    void Filesystem::PrintLua() {
        scripts.Refresh();
        scripts.PrintNames();
    }

    int Filesystem::ModifyFile(int fileNum, const std::filesystem::path& path) {
        if(!std::filesystem::is_regular_file(path))
            return 1;
        if(std::filesystem::file_size(path) == 0)
            return 2;
        std::string data;
        scripts.ChangeRows(fileNum, path, ReturnData, &data);
        CreateFile(path.parent_path(), path.filename().string(), data, true);
        return 0;
    }

    void Filesystem::RefreshLua() {
        scripts.Refresh();
    }

    void Filesystem::CreateFolder(std::filesystem::path path, const std::string& name, bool rewrite) {
        auto *current = new Directory(std::move(path));
        current->FetchDir();
        if (current->HasName(name) && rewrite)
            std::filesystem::remove_all(current->GetFullPath() + '/' + name);
        if (rewrite || !current->HasName(name))
            std::filesystem::create_directory(current->GetFullPath() + '/' + name);
    }

    void Filesystem::Remove(const std::filesystem::path& path, const std::string& name) {
        auto *current = new Directory(path);
        current->FetchDir();
        if(current->HasName(name))
            std::filesystem::remove_all(current->GetFullPath() + '/' + name);
    }

    void Filesystem::Rename(const std::filesystem::path &path, const std::string& oldName, const std::string& newName) {
        auto *current = new Directory(path);
        current->FetchDir();
        if(current->HasName(oldName) && !current->HasName(newName))
            std::filesystem::rename(current->GetFullPath() + '/' + oldName, current->GetFullPath() + '/' + newName);
    }

    void Filesystem::Replace(const std::filesystem::path &path, const std::string& name, const std::filesystem::path& newPath) {
        auto *current = new Directory(path);
        current->FetchDir();
        if(current->HasName(name) && std::filesystem::exists(newPath) && std::filesystem::is_directory(newPath))
            std::filesystem::rename(current->GetFullPath() + '/' + name, newPath.string() + '/' + name);
    }

    void Filesystem::Copy(const std::filesystem::path &path, const std::string& name, const std::filesystem::path& newPath, const std::string& newName) {
        auto *current = new Directory(path);
        if(current->HasName(name) && std::filesystem::exists(newPath))
            std::filesystem::copy(current->GetFullPath() + '/' + name, newPath.string() + '/' + newName, std::filesystem::copy_options::recursive);
        current->FetchDir();
    }

    std::vector<FObject *> Filesystem::GetByNameVec(const std::string &filename) {
        std::vector<FObject*> result;
        if(_current) {
            if(!_current->IsDirectory() && _current->GetFileName() == filename) {
                result.push_back(_current);
            } else {
                auto* dir = dynamic_cast<Directory *>(_current);
                std::vector<std::string> paths;
                int fMaskS = filename.find('*');
                int fMaskQ = filename.find('?');
                if(fMaskS != -1 && fMaskS == filename.find_last_of('*')) {
                    std::string start = filename.substr(0, fMaskS);
                    std::string end = filename.substr(fMaskS+1, filename.size()-1);
                    dir->FindInFilesVec(paths, start, end, true);
                }
                else if(fMaskQ != -1 && fMaskQ == filename.find_last_of('?')) {
                    std::string start = filename.substr(0, fMaskQ);
                    std::string end = filename.substr(fMaskQ+1, filename.size()-1);
                    dir->FindInFilesVec(paths, start, end, false);
                }
                else dir->FindInFilesVec(paths, filename);
                while (paths.begin() != paths.end()) {
                    result.push_back(OpenObject(*paths.begin()));
                    paths.erase(paths.begin());
                }
            }
        }

        return result;
    }
} // l6