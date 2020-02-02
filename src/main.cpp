#include "Application.hpp"

int main(int argc, char* argv[])
{
    new Application();
    g_application->SetCommandArgs(argc, argv);
    int ret = g_application->Run();

    return ret;
}