//
// Created by coltenus on 03.04.23.
//

#ifndef LAB6_SCRIPTS_H
#define LAB6_SCRIPTS_H

#include <vector>
#include <filesystem>
#include <functional>
#include <sol/sol.hpp>

namespace l6 {
    enum Mode : unsigned char {
        None = 0,
        Terminal,
        NewFile,
        ReturnData
    };

    class Scripts {
    private:
        std::vector<std::filesystem::path> _scripts;

    public:
        Scripts();
        ~Scripts();
        void Refresh();
        void PrintNames();
        bool ChangeRows(int pos, std::filesystem::path text, Mode mode, std::string* data = nullptr);
    };

} // l6

#endif //LAB6_SCRIPTS_H
