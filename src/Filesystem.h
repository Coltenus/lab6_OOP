//
// Created by coltenus on 22.03.23.
//

#ifndef LAB6_FILESYSTEM_H
#define LAB6_FILESYSTEM_H

#include "file_objects/file_objects.h"
#include "Scripts.h"

#ifdef _WIN32
#define ROOT "C:\\"
#define UP ":\\"
#define SEP '\\'
#endif
#ifdef __linux__
#define ROOT "/"
#define UP ROOT
#define SEP '/'
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#endif

namespace l6 {

    class Filesystem {
    private:
        FObject* _current;
        FObject* GetNameIfCurrent(const std::string& path);
        Scripts scripts;

    public:
        Filesystem();
        explicit Filesystem(std::string path);
        ~Filesystem();
        void Print();
        void Open(std::string path);
        void Open(const std::string& path, bool inCurrent);
        void Clear();
        bool IsOpened();
        void GoUpper();
        void CreateFolder(const std::string& name, bool rewrite = false);
        static void CreateFolder(std::filesystem::path path, const std::string& name, bool rewrite = false);
        void Remove(const std::string& name);
        static void Remove(const std::filesystem::path& path, const std::string& name);
        void Rename(const std::string& oldName, const std::string& newName);
        static void Rename(const std::filesystem::path& path, const std::string& oldName, const std::string& newName);
        void Replace(const std::string& name, const std::filesystem::path& path);
        static void Replace(const std::filesystem::path& path, const std::string& name, const std::filesystem::path& newPath);
        void Copy(const std::string& name, const std::string& path, const std::string& newName);
        static void Copy(const std::filesystem::path &path, const std::string& name, const std::filesystem::path& newPath, const std::string& newName);
        [[nodiscard]] std::string GetCurrentPath() const;
        FObject* GetByPath(std::string path);
        FObject* GetByName(const std::string& filename);
        std::vector<FObject*> GetByNameVec(const std::string& filename);
        FObject* OpenObject(const std::string& path);
        static Directory* GetRoot();
        template<typename T>
        int CreateFile(const std::filesystem::path& path, std::string filename, std::uint32_t size, T* data, bool rewrite = false);
        int CreateFile(const std::filesystem::path& path, std::string filename, std::string data, bool rewrite = false);
        static int ModifyFile(const std::filesystem::path& path, std::uint32_t size);
        void PrintLua();
        void RefreshLua();
        int ModifyFile(int fileNum, const std::filesystem::path& path);
    };

    template<typename T>
    int Filesystem::CreateFile(const std::filesystem::path& path, std::string filename, std::uint32_t size, T *data, bool rewrite) {
        if(!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
            return 1;
        if(!rewrite && std::filesystem::exists(path.string() + SEP + filename))
            return 2;
        FILE* file = nullptr;
        fopen_s(&file, (path.string() + SEP + filename).c_str(), "wt");
        if(file) {
            int result = 4;
            if(data && size > 0) {
                fwrite(data, sizeof(T), size, file);
                result = 0;
            }
            fclose(file);
            return result;
        }
        return 3;
    }

} // l6

#endif //LAB6_FILESYSTEM_H
