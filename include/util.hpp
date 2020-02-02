#pragma once

#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"

namespace util
{

bool file_exists(const std::string& filepath);

std::ostream& print_error(std::ostream& out, std::string msg);

bool not_space(char c);

std::string format_time(const boost::posix_time::time_duration& time, const std::string& fmt);

std::vector<std::string> crop_text(const std::string& in, size_t w);
std::vector<std::string> crop_text_trim(const std::string& in, size_t w);
std::vector<std::string> crop_text_wrap(const std::string& in, size_t w);

std::vector<std::string> text_rect(size_t w, size_t h, char c);
std::vector<std::string> text_rect(const std::string& firstLine);

enum class RelPos { BEFORE, INSIDE, AFTER };
RelPos compute_relpos(const boost::posix_time::time_duration& time, const boost::posix_time::time_duration& origin);

}

std::ostream& operator<<(std::ostream& out, const std::vector<std::string>& text);
std::vector<std::string> operator|(const std::vector<std::string>& left, const std::vector<std::string>& right);