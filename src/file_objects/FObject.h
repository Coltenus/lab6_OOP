//
// Created by coltenus on 20.03.23.
//

#ifndef LAB6_FOBJECT_H
#define LAB6_FOBJECT_H

#include <filesystem>
#include "../utils/LException.h"
#include <sys/stat.h>
#include <iostream>

namespace l6 {

    class FObject {
    public:
        struct Date {
            int year, month, day, hour, minute;
            friend std::ostream& operator<<(std::ostream& os, const Date& date) {
                if(date.year != 1970 && date.month != 1 && date.day != 1 && date.hour && date.minute)
                    os << date.year << '.' << date.month << '.' << date.day << ' ' << date.hour << ':' << date.minute;
                else os << "(no date)";
                return os;
            }
        };

    private:
        std::filesystem::path _path;
        std::string _filename;
        int _level;
        bool _isDirectory;
        Date _crDate;

    public:
        FObject(const std::filesystem::path& path, int level, bool isDirectory)
        : _path(path), _filename(path.filename().string()), _level(level), _isDirectory(isDirectory) {
            if(!std::filesystem::exists(path))
                throw LException("There is no such file or directory.", 1);
            {
                char cmd[300];
                snprintf(cmd, 300, "stat '%s' | grep 'Birth:' | cut -c 9-24", absolute(path).c_str());
                char buf[17];
                std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
                fgets(buf, 17, pipe.get());
                _crDate = {};
                sscanf(buf, "%d-%d-%d %d:%d", &_crDate.year, &_crDate.month, &_crDate.day,
                       &_crDate.hour, &_crDate.minute);
            }
        }
        virtual ~FObject() = default;
        [[nodiscard]] std::filesystem::path GetPathObject() const { return _path; }
        [[nodiscard]] std::string GetFileName() const { return _filename; }
        [[nodiscard]] int GetLevel() const { return _level; }
        virtual void PrintName(bool check = true) { std::cout << _filename << ' ' << _crDate; }
        [[nodiscard]] std::string GetPath() const { return _path.relative_path().string(); }
        [[nodiscard]] std::string GetFullPath() const {
            if(!_path.filename().string().empty())
                return _path.parent_path().string() + '/' + _path.filename().string();
            else return _path.parent_path().string();
        }
        [[nodiscard]] bool IsDirectory() const { return _isDirectory; }
        [[nodiscard]] Date GetCreationDate() const { return _crDate; }
        [[nodiscard]] virtual std::uint64_t GetSize() = 0;
    };

} // l6

#endif //LAB6_FOBJECT_H
