#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "rang/rang.hpp"
#include "util.hpp"
#include "config.hpp"

// DELETE
// #include <cstdio>

#include "Application.hpp"


Application* g_application;

/*
 * Control flow:
 * 1. Read tasks file and sort dates
 * 2. Append to oldtasks and rewrite tasks, sort if necessary
 * 3. Find today's block
 * 4. Print it
*/

Application::Application()
{
    assert(!g_application);
    g_application = this;
}

Application::~Application()
{
    assert(g_application == this);
    g_application = nullptr;
}

void Application::SetCommandArgs(int count, char* args[])
{
    this->args = std::vector<std::string>(args, args + count);
}

int Application::Run()
{
    int error = Application::m_Init();
    if (error)
        return error;

    return m_Main();
}

int Application::m_Init()
{
    if (!util::file_exists(config::tasks_filepath)) {
        util::print_error(std::cerr, config::tasks_filepath + " does not exist");
        return -1;
    }

    return 0;
}

int Application::m_Main()
{
    std::list<Tasksheet> tsheets;
    
    // TODO: Error handling
    m_LoadTasksheets(tsheets);

    tsheets.sort();     // By date

    m_MoveOldTasksheetsOut(tsheets);
    m_WriteTasksheetsToDisk(tsheets);

    // TODO: make FindTodays... a general function.
    // Then find the todays and tomorrow's sheets
    // If either of them is missing. Create a blank sheet.
    // and write them out
    Tasksheet todaySheet = m_FindTodaysTasksheet(tsheets);
    
    if (!todaySheet.Valid()) {
        std::cerr << "No Tasks For Today";
        return 0;
    }

    std::stable_sort(todaySheet.tasks.begin(), todaySheet.tasks.end());
    
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    std::cout << "Today is " << now.date() << '\n';

    m_DrawColorfulTable(todaySheet, now.time_of_day());

    return 0;
}

void Application::m_DrawTimeLine(size_t w)
{
    std::cout << config::starSymbolColor;
    std::cout << config::starSymbol << ' '
              << util::format_time(boost::posix_time::second_clock::local_time().time_of_day(), "%H:%M")
              << "  " << std::string(w, '-') << '\n';
    util::reset_colors(std::cout);
}

void Application::m_DrawColorfulTable(const Tasksheet& today, const boost::posix_time::time_duration& now)
{
    std::string::size_type descLen = util::max_description_len(today);

    bool lineDrawn = false;
    for (auto entry : today.tasks) {
        // Position of the current entry relative to the time-line.
        util::RelPos relPos = util::compute_relpos(entry.deadline, now); 

        // TODO: dublication of two space offset
        if (!lineDrawn && relPos == util::RelPos::AFTER) {
            m_DrawTimeLine(descLen);
            lineDrawn = true;
        }

        std::vector<std::string> description = util::crop_text(entry.description, config::description_width);
        std::vector<std::string> output = util::text_rect(2, 1, ' ')
                                        | util::text_rect(util::format_time(entry.deadline, "%H:%M"))
                                        | util::text_rect(2, 1, ' ')
                                        | description;

        if (relPos == util::RelPos::INSIDE) {
            std::cout << config::starSymbolColorInter << config::starSymbol;
            util::reset_colors(std::cout);
            // HACK: Just because I want a star to be a different color, I essentially copy the entire output
            output[0] = std::string(output[0].begin() + 1, output[0].end());
            lineDrawn = true;
        }

        std::cout << config::colorScheme[entry.status][(int)relPos];
        std::cout << output;
        util::reset_colors(std::cout);
    }

    if (!lineDrawn)
        m_DrawTimeLine(descLen);
}

void Application::m_LoadTasksheets(std::list<Tasksheet>& tsheets)
{
    std::fstream tasksFile(config::tasks_filepath, std::ios_base::in);
    util::load_tasksheets(tasksFile, tsheets);
    tasksFile.close();
}

void Application::m_MoveOldTasksheetsOut(std::list<Tasksheet>& tsheets)
{
    std::fstream oldtasksFile(config::oldtasks_filepath, std::ios_base::app);
    boost::gregorian::date today = boost::gregorian::day_clock::local_day();

    // TODO: Check if works, changed just now from for()
    auto it = tsheets.begin();
    while (it != tsheets.end()) {
        if (it->date < today) {
            oldtasksFile << it->ToPlainText() << '\n';
            it = tsheets.erase(it);
        } else {
            ++it;
        }
    }

    oldtasksFile.close();
}

void Application::m_WriteTasksheetsToDisk(const std::list<Tasksheet>& tsheets)
{
    std::fstream tasksFile(config::tasks_filepath, std::ios_base::out | std::ios_base::trunc);
    for (auto sheet : tsheets)
        tasksFile << sheet.ToPlainText() << '\n';

    tasksFile.close();
}

Tasksheet Application::m_FindTodaysTasksheet(const std::list<Tasksheet>& tsheets)
{
    boost::gregorian::date today = boost::gregorian::day_clock::local_day();
    for (auto& sheet : tsheets) {
        if (sheet.date == today)
            return sheet;
    }

    return {};
}
