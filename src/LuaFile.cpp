//
// Created by coltenus on 03.04.23.
//

#include "LuaFile.h"

namespace l6 {
    LuaFile::LuaFile(std::filesystem::path path) {
        _lua.open_libraries(sol::lib::base);
        if(std::filesystem::exists(path))
        {
            _lua.do_file(path);
            _exist = true;
        }
        else _exist = false;
    }

    bool LuaFile::IsExist() {
        return _exist;
    }
} // l6