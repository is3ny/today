#pragma once

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

struct Task
{
    char status;
    boost::posix_time::time_duration deadline;
    std::string description;

};

bool operator<(const Task& a, const Task& b);

struct Tasksheet
{
    boost::gregorian::date date;
    std::vector<Task> tasks;

    std::string ToPlainText();

    bool Valid();

    bool operator<(const Tasksheet& a)
    {
        return date < a.date;
    }
};

namespace util
{

std::fstream& read_task(std::fstream& in, Task& task);
std::fstream& read_tasksheet(std::fstream& in, Tasksheet& sheet);
std::fstream& load_tasksheets(std::fstream& in, std::list<Tasksheet>& sheets);

size_t max_description_len(const Tasksheet& sheet);

}