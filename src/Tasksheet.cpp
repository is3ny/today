#include <sstream>
#include "util.hpp"
#include "config.hpp"

#include "Tasksheet.hpp"

bool operator<(const Task& a, const Task& b)
{
    return a.deadline < b.deadline;
}

std::string Tasksheet::ToPlainText()
{
    std::stringstream out;
    out << date << '\n';
    for (auto task : tasks)
        out << task.status << ' ' << util::format_time(task.deadline, "%H:%M") << ' ' << task.description << '\n';

    return out.str();
}

bool Tasksheet::Valid()
{
    return date != boost::gregorian::date(boost::date_time::not_a_date_time);
}

namespace util
{

std::fstream& read_task(std::fstream& in, Task& task)
{
    // The empty line isn't a task
    if (in.peek() == '\n' || in.eof()) {
        in.clear(std::ios::failbit);
        return in;
    }

    std::string timestamp;
    in >> task.status >> timestamp;

    // TODO:
    // Boost.DateTime's parse_delimited_time_duration won't compile :-(
    // So, this is an ugly hack
    int h, m;
    sscanf(timestamp.c_str(), "%d:%d", &h, &m);
    task.deadline = boost::posix_time::time_duration{h, m, 0};

    std::string desc;
    std::getline(in, desc);
    task.description = std::string(std::find_if(desc.begin(), desc.end(), util::not_space), desc.end());

    return in;
}

std::fstream& read_tasksheet(std::fstream& in, Tasksheet& sheet)
{
    sheet.tasks.clear();

    std::string x;
    in >> x;

    if (!in)
        return in;

    sheet.date = boost::gregorian::from_simple_string(x);
    in.ignore();    // the newline character that should go after the date

    Task task;
    while (read_task(in, task))
        sheet.tasks.push_back(task);

    in.clear();
    return in;
}

std::fstream& load_tasksheets(std::fstream& in, std::list<Tasksheet>& sheets)
{
    Tasksheet sheet;
    while (read_tasksheet(in, sheet))
        sheets.push_back(sheet);

    in.clear();
    return in;
}

size_t max_description_len(const Tasksheet& sheet)
{
    size_t maxlen = 0;
    for (auto& entry : sheet.tasks) 
        maxlen = std::max(maxlen, entry.description.size());
    maxlen = std::min(maxlen, config::description_width);

    return maxlen;
}

}