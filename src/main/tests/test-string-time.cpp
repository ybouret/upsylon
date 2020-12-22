#include "y/string/time.hpp"
#include "y/utest/run.hpp"
#include <cstdio>
#include <ctime>

using namespace upsylon;

Y_UTEST(string_time)
{
#if 0
    time_t timer;
    char buffer[64];
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    puts(buffer);
#endif
    std::cerr << "stamp: " << string_time::stamp() << std::endl;
}
Y_UTEST_DONE()

