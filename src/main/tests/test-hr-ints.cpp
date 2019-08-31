#include "y/code/hr-ints.hpp"
#include "y/utest/run.hpp"
#include "y/type/ints.hpp"
#include "y/string/convert.hpp"
#include <cstdio>

using namespace upsylon;

static inline void display_hr( const human_readable &hr )
{
    fprintf(stderr,"%8.2f%c\n", hr.value, hr.radix);
}

Y_UTEST(hr_ints)
{
    human_readable hr_max( limit_of<int64_t>::maximum );
    display_hr(hr_max);
    human_readable hr_min( limit_of<int64_t>::minimum );
    display_hr(hr_min);

    for(int i=1;i<argc;++i)
    {
        const int64_t  a = string_convert::to<unit_t>(argv[i]);
        human_readable hr(a);
        display_hr(hr);
    }
}
Y_UTEST_DONE()



