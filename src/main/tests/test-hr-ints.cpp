#include "y/code/hr-ints.hpp"
#include "y/utest/run.hpp"
#include "y/type/ints.hpp"
#include "y/string/convert.hpp"
#include <cstdio>

using namespace upsylon;



Y_UTEST(hr_ints)
{
    human_readable hr_max( limit_of<int64_t>::maximum );
    std::cerr << "hr_max=" << hr_max << std::endl;
    human_readable hr_min( limit_of<int64_t>::minimum );
    std::cerr << "hr_min=" << hr_min << std::endl;

    for(int i=1;i<argc;++i)
    {
        const int64_t  a = string_convert::to<unit_t>(argv[i]);
        human_readable hr(a);
        std::cerr << hr << std::endl;
    }
}
Y_UTEST_DONE()



