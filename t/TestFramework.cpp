#include "TestFramework.h"

#include <iostream>

using namespace std;

TestFramework::TestFramework(): n_planned(0), n_got(0), n_fail(0)
{
}

TestFramework::TestFramework(size_t n_tests): n_planned(n_tests), n_got(0), n_fail(0)
{
    if (!n_tests)
    {
        cerr << "zero number of planned tests" << endl;
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
        cerr << "planned " << n_planned << ", but only run " << n_got << endl;
        return false;
    }
    else
    {
        cout << n_got << " tests, " << n_fail << " failed" << endl;
        return (n_fail == 0);
    }
}

void TestFramework::ok(bool value, const char *desc)
{
    n_got++;
    if (value)
    {
        cout << "ok " << n_got << " - " << desc << endl;
    }
    else
    {
        cout << "NOT ok " << n_got << " - " << desc << endl;
        n_fail++;
    }
}

int main(int argc, char** argv)
{
    TestFramework t;
    bool t_re = t.run();
    exit(!t_re);
}
