//
// Created by coltenus on 03.04.23.
//

#ifndef LAB6_LUAFILE_H
#define LAB6_LUAFILE_H

#include <sol/sol.hpp>
#include <filesystem>

namespace l6 {

    class LuaFile {
    private:
        sol::state _lua;
        bool _exist;

    public:
        explicit LuaFile(std::filesystem::path path);
        ~LuaFile() = default;
        template<typename T>
        T operator()(T val, const std::string& function);
        bool IsExist();
    };

    template<typename T>
    T LuaFile::operator()(T val, const std::string& function) {
        T result;
        if(_exist){
            auto lua_result = _lua[function.c_str()](val);
            result = lua_result;
        }
        return result;
    }

} // l6

#endif //LAB6_LUAFILE_H
