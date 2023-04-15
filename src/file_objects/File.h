//
// Created by colte on 21.03.2023.
//

#ifndef LAB6_FILE_H
#define LAB6_FILE_H

#include "FObject.h"

namespace l6 {

    class File : public FObject {
    private:
        std::string _extension;
        Date _modDate;
        std::uint64_t _size;

    public:
        File(const std::filesystem::path& path, int level);
        explicit File(const std::filesystem::path& path);
        ~File() override = default;
        [[nodiscard]] std::string GetExtension() const;
        void PrintName(bool check = true) override;
        [[nodiscard]] Date GetModificationDate() const;
        [[nodiscard]] std::uint64_t GetSize() override;
    };

} // l6

#endif //LAB6_FILE_H
