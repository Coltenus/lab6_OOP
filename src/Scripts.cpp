//
// Created by coltenus on 03.04.23.
//

#include <fstream>
#include "Scripts.h"
#include "LuaFile.h"

#define SCRIPT_PATH "./scripts"

namespace l6 {
    Scripts::Scripts() {
        std::filesystem::create_directory(SCRIPT_PATH);
    }

    Scripts::~Scripts() {
        _scripts.clear();
    }

    void Scripts::Refresh() {
        _scripts.clear();

        if(std::filesystem::exists(SCRIPT_PATH)){
            for (const auto &entry: std::filesystem::directory_iterator(SCRIPT_PATH)) {
                if (entry.is_regular_file() && entry.path().filename().string().ends_with(".lua")) {
                    _scripts.push_back(entry.path());
                }
            }
        }
        else std::filesystem::create_directory(SCRIPT_PATH);
    }

    void Scripts::PrintNames() {
        int i = 1;
        printf("Here are the found scripts:\n");
        for(auto& el: _scripts) {
            printf("\t%d) %s\n", i, el.filename().string().c_str());
            i++;
        }
        printf("\n");
    }

    bool Scripts::ChangeRows(int pos, std::filesystem::path text, Mode mode, std::string* data) {
        const char* FunctionName = "rows";
        bool result = false;
        static auto GetNewName = [](std::filesystem::path path) {
            std::filesystem::path textC = path;
            std::string filename = textC.filename().string();
            std::string ext = textC.extension().string();
            int extP = filename.rfind(ext);
            filename.erase(filename.begin()+extP, filename.end());
            textC.replace_filename(filename+"_new"+ext);
            return textC;
        };
        pos--;

        if(pos >= 0 && pos < _scripts.size() && std::filesystem::exists(_scripts[pos]))
            result = true;
        else return false;

        if(mode == ReturnData && data == nullptr)
            return false;

        if(mode != None){
            char buffer[100];
            auto path = std::string(std::filesystem::absolute(text).string());
            auto lua = new LuaFile(_scripts[pos]);
            std::ifstream rFile(path);
            switch (mode) {
                case Terminal:
                    printf("Output:\n");
                    break;
                case NewFile:
                    {
                        std::filesystem::path textC = GetNewName(text);
                        std::ofstream wFile(textC, std::ofstream::trunc);
                    }
                    break;
                case ReturnData:
                    *data = "";
                    break;
            }
            while (lua->IsExist() && rFile.is_open() && !rFile.eof()) {
                rFile.getline(buffer, 100);
                try{
                    switch (mode) {
                        case Terminal: {
                            std::string formatT = "%s";
                            if (!rFile.eof())
                                formatT += '\n';
                            printf(formatT.c_str(), (*lua)(std::string(buffer), FunctionName).c_str());
                        }
                            break;
                        case NewFile: {
                            std::filesystem::path textC = GetNewName(text);
                            std::ofstream wFile(textC, std::ofstream::app);
                            auto bufStr = (*lua)(std::string(buffer), FunctionName);
                            if (!rFile.eof())
                                bufStr += '\n';
                            wFile.write(bufStr.c_str(), bufStr.size());
                            wFile.close();
                        }
                            break;
                        case ReturnData:
                            *data += (*lua)(std::string(buffer), FunctionName);
                            if (!rFile.eof())
                                *data += '\n';
                            break;
                    }
                }
                catch (sol::error& err) {
//                    std::cout << err.what() << '\n';
                }
            }
            rFile.close();
            delete lua;
        }

        return result;
    }
} // l6