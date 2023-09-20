#pragma once

#include <string>
#include <utility>

struct Position {
    size_t index = 0;
    size_t line_number = 0;
    size_t column = 0;
    std::string file_name;
    std::string file_contents;

    Position(std::string file_name, std::string file_contents)
        : file_name(std::move(file_name)), file_contents(file_contents) { }

    void advance(char current_character = '\0') {
        this->index++;
        this->column++;

        if(current_character == '\n') {
            this->line_number++;
            this->column = 0;
        }
    }
};