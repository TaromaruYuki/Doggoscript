#pragma once

#include <config/DoggoscriptConfig.h>
#include <string>

#ifdef HAVE_FORMAT
#include <format>
#else
#include <sstream>
#endif

#include "position.hpp"

class BaseError {
    Position start_pos;
    Position end_pos;
    std::string error_name;
    std::string details;

public:
    BaseError(Position start_pos, Position end_pos, std::string error_name, std::string details)
            : start_pos(start_pos), end_pos(end_pos), error_name(error_name), details(details) {}

    void str_replace(std::string &str, const std::string &from, const std::string &to) {
        size_t pos_start = str.find(from);
        if (pos_start == std::string::npos)
            return;
        str.replace(pos_start, from.length(), to);
    }

    std::string string_with_arrows() {
        std::string result;
        std::string text = this->start_pos.file_contents;

        int idx_start = std::max((int) text.rfind('\n', this->start_pos.index), 0);
        int idx_end = (int) text.find('\n', idx_start + 1);
        if (idx_end < 0)
            idx_end = (int) text.length();

        int line_count = this->end_pos.line_number - this->start_pos.line_number + 1;
        for (int i = 0; i < line_count; i++) {
            std::string line = text.substr(idx_start, idx_end - idx_start);

            int col_start = this->start_pos.column - 1;
            if (i != 0)
                col_start = 0;
            int col_end = this->end_pos.column - 1;

            if (i != line_count - 1)
                col_end = line.length() - 1;

            result += line + "\n";

            for (int j = 0; j < col_start; j++)
                result += " ";

            for (int j = col_start; j <= col_end; j++)
                result += "^";

            idx_start = idx_end;
            idx_end = (int) text.find('\n', idx_start + 1);
            if (idx_end < 0) idx_end = (int) text.length();
        }

        str_replace(result, "\t", "");

        return result;
    }

    std::string str() {
#ifdef HAVE_FORMAT
        return std::format(
                "{}: {}\nFile {}, line {}\n\n{}",
                this->error_name,
                this->details,
                this->start_pos.file_name,
                this->start_pos.line_number + 1,
                this->string_with_arrows()
        );
#else
        std::stringstream format;
        format << this->error_name << ": " << this->details << "\nFile " << this->start_pos.file_name << ", line "
               << this->start_pos.line_number + 1 << "\n\n" << this->string_with_arrows();

        return format.str();
#endif
    }
};

class IllegalCharacterError : public BaseError {
public:
    IllegalCharacterError(Position start_pos, Position end_pos, char character)
            : BaseError(start_pos, end_pos, "Illegal Character", "'" + std::to_string(character) + "'") {}
};

class ExpectedCharacterError : public BaseError {
public:
    ExpectedCharacterError(Position start_pos, Position end_pos, char character)
            : BaseError(start_pos, end_pos, "Expected Character", "'" + std::to_string(character) + "'") {}
};

class InvalidSyntaxError : public BaseError {
public:
    InvalidSyntaxError(Position start_pos, Position end_pos, std::string details)
            : BaseError(start_pos, end_pos, "Invalid Syntax", details) {}
};

class ArithmeticError : public BaseError {
public:
    ArithmeticError(Position start_pos, Position end_pos, std::string details)
            : BaseError(start_pos, end_pos, "Arithmetic Error", details) {}
};

class IllegalOperationError : public BaseError {
public:
    IllegalOperationError(Position start_pos, Position end_pos, std::string details)
            : BaseError(start_pos, end_pos, "Illegal Operation", details) {}
};

class NameError : public BaseError {
public:
    NameError(Position start_pos, Position end_pos, std::string details)
            : BaseError(start_pos, end_pos, "Name Error", details) {}
};

class ArgumentError : public BaseError {
public:
    ArgumentError(Position start_pos, Position end_pos, std::string details)
            : BaseError(start_pos, end_pos, "Argument Error", details) {}
};

class TypeError : public BaseError {
public:
    TypeError(Position start_pos, Position end_pos, std::string details)
            : BaseError(start_pos, end_pos, "Type Error", details) {}
};

class FileNotFoundError : public BaseError {
public:
    FileNotFoundError(Position start_pos, Position end_pos, std::string details)
            : BaseError(start_pos, end_pos, "File Not Found", details) {}
};
