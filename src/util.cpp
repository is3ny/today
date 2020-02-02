#include <fstream>
#include <sstream>
#include <cctype>
#include "boost/date_time/time_facet.hpp"
#include "boost/date_time.hpp"
#include "config.hpp"

#include "util.hpp"

namespace util
{

bool file_exists(const std::string& filepath)
{
    std::fstream f(filepath, std::ios_base::in);
    bool open = f.is_open();
    f.close();
    return open;
}

std::ostream& print_error(std::ostream& out, std::string msg)
{
    out << "today::error    " << msg << '\n';

    return out;
}

bool not_space(char c)
{
    return !isspace(c);
}

std::string format_time(const boost::posix_time::time_duration& time, const std::string& fmt)
{
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet{fmt.data()};

    // Since we don't care about the date, it's set to an arbitrary valid value
    boost::posix_time::ptime posixT{boost::gregorian::date{boost::date_time::max_date_time}, time};

    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));      // Apply facet
    ss << posixT;                                   // Passing just time wouldn't work

    return ss.str();
}

// Driver function that depending on the wrap_type set in the config
// calls an appropriate cropping function
std::vector<std::string> crop_text(const std::string& in, size_t max_chars)
{
    switch (config::wrap_type) {
        case config::TextWrap::TRIM:
            return crop_text_trim(in, max_chars);
        case config::TextWrap::WRAP:
            return crop_text_wrap(in, max_chars);
        default:
            return text_rect(in);
    }
}

// Trims longer than w string to a w - 3 lenght and appends three dots.
// Example:
// crop_text_trim("123456", 5) -> "12..."
std::vector<std::string> crop_text_trim(const std::string& in, size_t w)
{
    if (in.size() > w)
        return text_rect(in.substr(0, w - 3) + "...");
    return text_rect(in);
}

// Wraps text without respect to anything.
// For instance:
// crop_text_wrap("This is an example.", 6) -> { "This i", "s an e", "xample", "." }
std::vector<std::string> crop_text_wrap(const std::string& in, size_t w)
{
    std::vector<std::string> out;
    int start = 0;
    while (in.size() - start > w) {
        out.push_back(in.substr(start, w));
        start += w;
    }

    out.push_back(in.substr(start));
    return out;
}

// Creates an array that depicts a rectangle consisting solely of some character
std::vector<std::string> text_rect(size_t w, size_t h, char c)
{
    return std::vector<std::string>(h, std::string(w, c));
}
   
// Converter function from string to an array with this string
std::vector<std::string> text_rect(const std::string& firstLine)
{
    std::vector<std::string> ret;
    ret.push_back(firstLine);

    return ret;
}

RelPos compute_relpos(const boost::posix_time::time_duration& time, const boost::posix_time::time_duration& origin)
{
    
    if (time.hours() == origin.hours() && time.minutes() == origin.minutes())
        return RelPos::INSIDE;
    else if (time > origin)
        return RelPos::AFTER;
    return RelPos::BEFORE;
}

} // namespace util

std::ostream& operator<<(std::ostream& out, const std::vector<std::string>& text)
{
    for (auto& line : text)
        out << line << '\n';

    return out;
}

// Allows for seamless horizontal concatenation of character pictures.
std::vector<std::string> operator|(const std::vector<std::string>& left, const std::vector<std::string>& right)
{
    std::string::size_type maxlen = 0;
    for (auto& s : left)
        maxlen = std::max(maxlen, s.size());

    std::vector<std::string> out;
    for (int i = 0; i < right.size(); ++i) {
        auto left_part = i < left.size() ? left[i] : std::string();
        out.push_back(left_part + std::string(maxlen - left_part.size(), ' ') + right[i]);
    }

    for (int i = right.size(); i < left.size(); ++i)
        out.push_back(left[i]);

    return out;
}