//
// Created by coltenus on 21.03.23.
//

#ifndef LAB6_LEXCEPTION_H
#define LAB6_LEXCEPTION_H

#include <exception>

namespace l6 {

    class LException : public std::exception {
    private:
        int _opcode;
        const char* _msg;

    public:
        LException(const char* message, int opcode) : _msg(message), _opcode(opcode) {}
        virtual const char* what() const noexcept {
            return _msg;
        }
        int opcode() const noexcept {
            return _opcode;
        }
    };

} // l6

#endif //LAB6_LEXCEPTION_H
