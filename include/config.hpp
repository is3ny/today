#pragma once

#include <string>
#include <map>
#include <array>
#include "TTYFormat.hpp"

namespace config 
{

extern std::string tasks_filepath, oldtasks_filepath;

extern char starSymbol, exclSymbol;

enum class TextWrap { TRIM, WRAP };
extern size_t description_width;
extern TextWrap wrap_type;

extern TTYFormat starSymbolColor;
extern TTYFormat starSymbolColorInter;
extern std::map<char, std::array<TTYFormat, 3>> colorScheme;

}