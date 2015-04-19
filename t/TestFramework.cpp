#include "TestFramework.h"

#include <treejuce/Logger.h>
#include <treejuce/Result.h>
#include <treejuce/String.h>

using namespace treejuce;

TestFramework::TestFramework(): n_planned(0), n_got(0), n_fail(0)
{
}

TestFramework::TestFramework(size_t n_tests): n_planned(n_tests), n_got(0), n_fail(0)
{
    if (!n_tests)
    {
        Logger::writeToLog("zero number of planned tests");
        exit(EXIT_FAILURE);
    }
}

TestFramework::~TestFramework()
{
}

bool TestFramework::run()
{
    content();
    if (n_planned && n_planned != n_got)
    {
        Logger::writeToLog("planned " + String(n_planned) + ", but only run " + String(n_got));
        return false;
    }
    else
    {
        Logger::writeToLog(String(n_got) + " tests, " + String(n_fail) + " failed");
        return (n_fail == 0);
    }
}

void TestFramework::ok(bool value, const char *desc)
{
    n_got++;
    if (value)
    {
        Logger::writeToLog("ok " + String(n_got) + " - " + desc);
    }
    else
    {
        Logger::writeToLog("NOT ok " + String(n_got) + " - " + desc);
        n_fail++;
    }
}

void TestFramework::ok(const treejuce::Result& result, const char* desc)
{
    n_got++;
    if (result)
    {
        Logger::writeToLog("ok " + String(n_got) + " - " + desc);
    }
    else
    {
        Logger::writeToLog("NOT ok " + String(n_got) + " - " + result.getErrorMessage());
        n_fail++;
    }
}

int main(int argc, char** argv)
{
    TestFramework t;
    bool t_re = t.run();
    exit(!t_re);
}
