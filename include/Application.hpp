#pragma once

#include <vector>
#include <string>
#include <list>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Tasksheet.hpp"

extern class Application* g_application;

class Application
{
public:
    Application();
    ~Application();

    void SetCommandArgs(int count, char* args[]);

    int Run();

private:
    std::vector<std::string> args;

    int m_Init();
    int m_Main();

    void m_DrawTimeLine(size_t w);
    void m_DrawColorfulTable(const Tasksheet& today, const boost::posix_time::time_duration& now);

    void m_LoadTasksheets(std::list<Tasksheet>& tsheets);
    void m_MoveOldTasksheetsOut(std::list<Tasksheet>& tsheets);
    void m_WriteTasksheetsToDisk(const std::list<Tasksheet>& tsheets);
    Tasksheet m_FindTodaysTasksheet(const std::list<Tasksheet>& tsheets);
};