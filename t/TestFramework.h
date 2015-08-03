#ifndef TREEFACE_TEST_FRAMEWORK
#define TREEFACE_TEST_FRAMEWORK

#include "treeface/common.h"

#include <treecore/Common.h>

#include <cstdint>
#include <cstdlib>

#include <iostream>
#include <cmath>

namespace treecore {
class Result;
} // namespace treecore

#define OK(arg) ok(arg, EXPAND_AND_STRINGIFY(arg))
#define IS(arg1, arg2) is(arg1, arg2, EXPAND_AND_STRINGIFY(arg1) " == " EXPAND_AND_STRINGIFY(arg2))
#define IS_EPSILON(arg1, arg2) is_epsilon(arg1, arg2, EXPAND_AND_STRINGIFY(arg1) " ~~ " EXPAND_AND_STRINGIFY(arg2))
#define GT(arg1, arg2) gt(arg1, arg2, EXPAND_AND_STRINGIFY(arg1) " > " EXPAND_AND_STRINGIFY(arg2))
#define GE(arg1, arg2) ge(arg1, arg2, EXPAND_AND_STRINGIFY(arg1) " >= " EXPAND_AND_STRINGIFY(arg2))
#define LT(arg1, arg2) lt(arg1, arg2, EXPAND_AND_STRINGIFY(arg1) " < " EXPAND_AND_STRINGIFY(arg2))
#define LE(arg1, arg2) le(arg1, arg2, EXPAND_AND_STRINGIFY(arg1) " <= " EXPAND_AND_STRINGIFY(arg2))

class TestFramework
{
public:
    TestFramework();
    TestFramework(size_t n_tests);
    ~TestFramework();

    void ok(bool value, const char* desc);
    void ok(const treecore::Result& result, const char* desc);

    template<typename T1, typename T2>
    void is(T1 value, T2 expect, const char* desc);

    template<typename T1, typename T2>
    void is_epsilon(T1 value, T2 expect, const char* desc, T2 rate = 10000);

    template<typename T1, typename T2>
    void gt(T1 a, T2 b, const char* desc);

    template<typename T1, typename T2>
    void ge(T1 a, T2 b, const char* desc);

    template<typename T1, typename T2>
    void lt(T1 a, T2 b, const char* desc);

    template<typename T1, typename T2>
    void le(T1 a, T2 b, const char* desc);

    bool run();

protected:
    void content();
    size_t n_planned;
    size_t n_got;
    size_t n_fail;
};

template<typename T1, typename T2>
void TestFramework::is(T1 value, T2 expect, const char* desc)
{
    n_got++;
    if (value == expect)
    {
        std::cout << "ok " << n_got << " - " << desc << std::endl;
    }
    else
    {
        std::cout << "NOT ok " << n_got << " - " << desc << std::endl
                  <<"  got " << value <<", expect "<< expect << "  " << std::endl;
        n_fail++;
    }
}

template<typename T1, typename T2>
void TestFramework::is_epsilon(T1 value, T2 expect, const char* desc, T2 rate)
{
    n_got++;
    if (std::abs(value-expect) <= std::abs(1.0/rate))
    {
        std::cout << "ok " << n_got << " - " << desc << std::endl;
    }
    else
    {
        std::cout << "NOT ok " << n_got << " - " << desc << std::endl
                  <<"  got " << value <<", expect "<< expect << "  " << std::endl;
        n_fail++;
    }
}

template<typename T1, typename T2>
void TestFramework::gt(T1 a, T2 b, const char* desc)
{
    n_got++;
    if (a > b)
    {
        std::cout << "ok " << n_got << " - " << desc << std::endl;
    }
    else
    {
        std::cout << "NOT ok " << n_got << " - " << desc << std::endl
                  << "  got " << a << " and " << b << std::endl;
        n_fail++;
    }
}

template<typename T1, typename T2>
void TestFramework::ge(T1 a, T2 b, const char* desc)
{
    n_got++;
    if (a >= b)
    {
        std::cout << "ok " << n_got << " - " << desc << std::endl;
    }
    else
    {
        std::cout << "NOT ok " << n_got << " - " << desc << std::endl
                  << "  got " << a << " and " << b << std::endl;
        n_fail++;
    }
}

template<typename T1, typename T2>
void TestFramework::lt(T1 a, T2 b, const char* desc)
{
    n_got++;
    if (a < b)
    {
        std::cout << "ok " << n_got << " - " << desc << std::endl;
    }
    else
    {
        std::cout << "NOT ok " << n_got << " - " << desc << std::endl
                  << "  got " << a << " and " << b << std::endl;
        n_fail++;
    }
}

template<typename T1, typename T2>
void TestFramework::le(T1 a, T2 b, const char* desc)
{
    n_got++;
    if (a <= b)
    {
        std::cout << "ok " << n_got << " - " << desc << std::endl;
    }
    else
    {
        std::cout << "NOT ok " << n_got << " - " << desc << std::endl
                  << "  got " << a << " and " << b << std::endl;
        n_fail++;
    }
}

#endif // TREEFACE_TEST_FRAMEWORK
