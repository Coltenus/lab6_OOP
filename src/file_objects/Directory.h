//
// Created by colte on 21.03.2023.
//

#ifndef LAB6_DIRECTORY_H
#define LAB6_DIRECTORY_H

#include <set>
#include "FObject.h"

namespace l6 {

    class Directory : public FObject {
    private:
        std::set<FObject*> files;

    public:
        Directory(std::filesystem::path path, std::string filename, int level);
        ~Directory() override;
        void FetchDir();
        void PrintName() override;
    };

} // l6

#endif //LAB6_DIRECTORY_H
