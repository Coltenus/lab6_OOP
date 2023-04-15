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
        std::unordered_map<std::string, FObject*> _files;
        std::vector<std::string> _fileNames;
        std::string FindByPathStack(std::stack<std::string>& path);

    public:
        Directory(const std::filesystem::path& path, int level);
        explicit Directory(const std::filesystem::path& path);
        ~Directory() override;
        void FetchDir();
        void PrintName(bool check = true) override;
        // Exact name
        std::string FindInFiles(const std::string& filename);
        // Using mask
        std::string FindInFiles(const std::string& start, const std::string& end, bool greater);
        // Exact name
        void FindInFilesVec(std::vector<std::string>& files, const std::string& filename);
        // Using mask
        void FindInFilesVec(std::vector<std::string>& files, const std::string& start, const std::string& end, bool greater);
        std::string FindByPath(std::string path);
        void ClearFilesData();
        bool HasName(const std::string& name) const;
        std::uint64_t GetSize() override;
        std::uint16_t GetAllInnerObjectsCount();
        std::uint16_t GetInnerObjectsCount();
    };

} // l6

#endif //LAB6_DIRECTORY_H
