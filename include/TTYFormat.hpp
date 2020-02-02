#pragma once

#include <iostream>
#include "rang/rang.hpp"

/* Helper class that encapsates data about console font style (color, boldness...)
 * provided by rang library.
 */
struct TTYFormat
{
    rang::fg fg = rang::fg::reset;
    rang::fgB brightFg = rang::fgB::reset;
    rang::bg bg = rang::bg::reset;
    rang::bgB brightBg = rang::bgB::reset;
    rang::style style = rang::style::reset;

    TTYFormat() = default;
    TTYFormat(rang::fg fg, rang::fgB fgB, rang::bg bg, rang::bgB bgB, rang::style style)
        : fg(fg), brightFg(fgB), bg(bg), brightBg(bgB), style(style) {} 
};

std::ostream& operator<<(std::ostream& out, const TTYFormat& fmt);

namespace util
{

inline void reset_colors(std::ostream& out)
{
    out << TTYFormat();
}

} // namespace util