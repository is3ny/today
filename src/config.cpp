#include "rang/rang.hpp"

#include "config.hpp"

namespace config
{

std::string tasks_filepath = "/home/isabelle/tasks";
std::string oldtasks_filepath = "/home/isabelle/oldtasks";

char starSymbol = '*';
char exclSymbol = ' ';

size_t description_width = 80;
TextWrap wrap_type = TextWrap::WRAP;

using namespace rang;
TTYFormat starSymbolColor{ fg::reset, fgB::red, bg::reset, bgB::reset, style::reset };
TTYFormat starSymbolColorInter{ fg::black, fgB::reset, bg::red, bgB::reset, style::reset };
std::map<char, std::array<TTYFormat, 3>> colorScheme = {
    { '#', {} },
    { 'O', {                                            // Completed task
        TTYFormat{ fg::reset, fgB::green, bg::reset, bgB::reset, style::reset },
        TTYFormat{ fg::reset, fgB::green, bg::reset, bgB::reset, style::reset },
        TTYFormat{ fg::reset, fgB::green, bg::reset, bgB::reset, style::reset }
    } },
    { 'X', {                                            // Failed task
        TTYFormat{ fg::red, fgB::reset, bg::reset, bgB::reset, style::reset },
        TTYFormat{ fg::reset, fgB::red, bg::reset, bgB::reset, style::reset },
        TTYFormat{ fg::reset, fgB::reset, bg::red, bgB::reset, style::reset }
    } },
    { '!', {                                            // Completed after the deadline
        TTYFormat{ fg::black, fgB::reset, bg::green, bgB::reset, style::reset },
        TTYFormat{ fg::reset, fgB::green, bg::reset, bgB::reset, style::reset },
        TTYFormat{ fg::black, fgB::reset, bg::green, bgB::reset, style::reset }
    } },
    { 'C', {                                            // Completed after the deadline
        TTYFormat{ fg::reset, fgB::reset, bg::reset, bgB::reset, style::crossed },
        TTYFormat{ fg::reset, fgB::blue, bg::reset, bgB::reset, style::crossed },
        TTYFormat{ fg::reset, fgB::blue, bg::reset, bgB::reset, style::crossed }
    } }
};

}
