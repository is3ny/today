#include "TTYFormat.hpp"

std::ostream& operator<<(std::ostream& out, const TTYFormat& fmt)
{
    if (fmt.brightFg != rang::fgB::reset)
        out << fmt.brightFg;
    else
        out << fmt.fg;

    if (fmt.brightBg != rang::bgB::reset)
        out << fmt.brightBg;
    else
        out << fmt.bg;

    if (fmt.style != rang::style::reset)
	out << fmt.style;

    // UGLY: When I wrote this I didn't know that style::reset resets color also
    if (fmt.brightFg == rang::fgB::reset
        && fmt.brightBg == rang::bgB::reset
        && fmt.fg == rang::fg::reset
        && fmt.bg == rang::bg::reset)
        out << fmt.style;

    return out;    
}
