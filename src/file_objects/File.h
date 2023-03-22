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

    public:
        File(std::filesystem::path path, int level);
        File(std::filesystem::path path);
        ~File() override = default;
        std::string GetExtension();
        std::string GetNameOnly();
        void PrintName() override;
    };

} // l6

#endif //LAB6_FILE_H
