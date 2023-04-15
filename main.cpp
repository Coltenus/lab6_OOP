#include <iostream>
#if __linux__
#include "src/Filesystem.h"
#endif

int main() {
#if __linux__
    l6::Filesystem fs;
    bool end = false;
    while (!end) {
        std::cout << "Select action:\n"
                     "1)  Open file with path\n"
                     "2)  Open file with file name\n"
                     "3)  Open root\n"
                     "4)  Check if opened object is directory\n"
                     "5)  Get attributes of opened object\n"
                     "6)  Create folder\n"
                     "7)  Create file\n"
                     "8)  Modify file\n"
                     "9)  Remove object\n"
                     "10) Rename object\n"
                     "11) Replace object\n"
                     "12) Copy object\n"
                     "13) Find all files with mask(only one mask, ? - one symbol or * - >= 1)\n"
                     "14) Print opened object\n"
                     "0) Exit\n"
                     ": ";
        std::uint8_t opt;
        char buf[3], data[120];
        scanf("%s", buf);
        try{
            opt = std::stoi(buf);
        }
        catch (...) {
            opt = 15;
        }
        switch (opt) {
            case 0:
                end = true;
                break;
            case 1:
                std::cout << "Enter path: ";
                scanf("%s", data);
                fs.Open(data, std::string(data).starts_with('.'));
                break;
            case 2:
                std::cout << "Enter file name: ";
                scanf("%s", data);
                fs.Open(fs.GetByName(data)->GetFullPath());
                break;
            case 3:
                fs.Open(l6::Filesystem::GetRoot()->GetFullPath());
                break;
            case 4:
                if(fs.IsDirectory())
                    std::cout << "Directory is opened\n";
                else std::cout << "File is opened\n";
                break;
            case 5:
                fs.Print(false);
                break;
            case 6:
                std::uint8_t opt6;
                std::cout << "Select option(0 - default, 1 - in current, 2 - by path)\n: ";
                scanf("%s", buf);
                try {
                    opt6 = std::stoi(buf);
                }
                catch (...) {
                    opt6 = 0;
                }
                if(opt6 == 1 || opt6 == 2) {
                    std::cout << "Enter folder name: ";
                    scanf("%s", data);
                }
                if(opt6 == 1)
                    fs.CreateFolder(data);
                else if(opt6 == 2){
                    char data6[120];
                    std::cout << "Enter path: ";
                    scanf("%s", data6);
                    l6::Filesystem::CreateFolder(std::filesystem::path(data6), std::string(data));
                }
                break;
            case 7:
                char text7[400], data7[120];
                std::cout << "Enter path: ";
                scanf("%s", data7);
                std::cout << "Enter file name: ";
                scanf("%s\n", data);
                std::fgets(text7, 400, stdin);
                fs.CreateFile(data7, data, text7);
                break;
            case 8:
                std::uint8_t opt8;
                std::cout << "Select option(0 - default, 1 - change size, 2 - modify with lua file)\n: ";
                scanf("%s", buf);
                try {
                    opt8 = std::stoi(buf);
                }
                catch (...) {
                    opt8 = 0;
                }
                if(opt8 == 1 || opt8 == 2) {
                    std::cout << "Enter full path: ";
                    scanf("%s", data);
                }
                if(opt8 == 1) {
                    std::cout << "File size: " << file_size(std::filesystem::path(data)) << '\n';
                    std::uint32_t size8;
                    std::cout << "Select size(0 - default)\n: ";
                    scanf("%s", buf);
                    try {
                        size8 = std::stoi(buf);
                    }
                    catch (...) {
                        size8 = 0;
                    }
                    l6::Filesystem::ModifyFile(data, size8);
                }
                else if(opt8 == 2) {
                    fs.PrintLua();
                    std::uint8_t file8;
                    std::cout << "Select file(0 - default)\n: ";
                    scanf("%s", buf);
                    try {
                        file8 = std::stoi(buf);
                    }
                    catch (...) {
                        file8 = 0;
                    }
                    fs.ModifyFile(file8, data);
                }
                break;
            case 9:
                std::uint8_t opt9;
                std::cout << "Select option(0 - default, 1 - in current, 2 - by path\n: ";
                scanf("%s", buf);
                try {
                    opt9 = std::stoi(buf);
                }
                catch (...) {
                    opt9 = 0;
                }
                if(opt9 == 1 || opt9 == 2) {
                    std::cout << "Enter file name: ";
                    scanf("%s", data);
                }
                if(opt9 == 1) {
                    fs.Remove(data);
                }
                else if(opt9 == 2){
                    char data9[120];
                    std::cout << "Enter path: ";
                    scanf("%s", data9);
                    l6::Filesystem::Remove(data9, data);
                }
                break;
            case 10:
                char name10[40];
                std::uint8_t opt10;
                std::cout << "Select option(0 - default, 1 - in current, 2 - by path\n: ";
                scanf("%s", buf);
                try {
                    opt10 = std::stoi(buf);
                }
                catch (...) {
                    opt10 = 0;
                }
                if(opt10 == 1 || opt10 == 2) {
                    std::cout << "Enter file name: ";
                    scanf("%s", data);
                    std::cout << "Enter new file name: ";
                    scanf("%s", name10);
                }
                if(opt10 == 1) {
                    fs.Rename(data, name10);
                }
                else if(opt10 == 2){
                    char data10[120];
                    std::cout << "Enter path: ";
                    scanf("%s", data10);
                    l6::Filesystem::Rename(data10, data, name10);
                }
                break;
            case 11:
                char path11[120];
                std::uint8_t opt11;
                std::cout << "Select option(0 - default, 1 - in current, 2 - by path\n: ";
                scanf("%s", buf);
                try {
                    opt11 = std::stoi(buf);
                }
                catch (...) {
                    opt11 = 0;
                }
                if(opt11 == 1 || opt11 == 2) {
                    std::cout << "Enter file name: ";
                    scanf("%s", data);
                    std::cout << "Enter new path: ";
                    scanf("%s", path11);
                }
                if(opt11 == 1) {
                    fs.Replace(data, path11);
                }
                else if(opt11 == 2){
                    char data11[120];
                    std::cout << "Enter path: ";
                    scanf("%s", data11);
                    l6::Filesystem::Replace(data11, data, path11);
                }
                break;
            case 12:
                char path12[120], name12[40];
                std::uint8_t opt12;
                std::cout << "Select option(0 - default, 1 - in current, 2 - by path\n: ";
                scanf("%s", buf);
                try {
                    opt12 = std::stoi(buf);
                }
                catch (...) {
                    opt12 = 0;
                }
                if(opt12 == 1 || opt12 == 2) {
                    std::cout << "Enter file name: ";
                    scanf("%s", data);
                    std::cout << "Enter new path: ";
                    scanf("%s", path12);
                    std::cout << "Enter new file name: ";
                    scanf("%s", name12);
                }
                if(opt12 == 1) {
                    fs.Copy(data, path12, name12);
                }
                else if(opt12 == 2){
                    char data12[120];
                    std::cout << "Enter path: ";
                    scanf("%s", data12);
                    l6::Filesystem::Copy(data12, data, path12, name12);
                }
                break;
            case 13:
                {
                    std::cout << "Enter file name: ";
                    scanf("%s", data);
                    auto vec13 = fs.GetByNameVec(data);
                    std::cout << "Found:\n";
                    for(auto& el: vec13)
                        std::cout << el->GetFullPath() << '\n';
                }
                break;
            case 14:
                fs.Print();
                break;
        }
        if(opt > 0 && opt <= 14)
            system("read -s");
    }
#else
    std::cout << "This program requires some Linux system libraries.\n";
#endif
    return 0;
}

