#include "y/mpl/pfq.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

Y_UTEST(pfq)
{
    size_t n = 0;
    size_t d = 1;

    if( argc > 1 )
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }

    if( argc > 2 )
    {
        d = string_convert::to<size_t>(argv[2],"d");
    }



    mpl::pfq Q(n,d);
    std::cerr << "Q=" << Q << std::endl;

    if( argc > 3 )
    {
        const mpl::pfd mul_by = string_convert::to<size_t>(argv[3],"mul_by");
        Q.mul_by(mul_by);
        std::cerr << "Q*" << mul_by.value() << "=" << Q << std::endl;
    }

    if( argc > 4 )
    {
        const mpl::pfd div_by = string_convert::to<size_t>(argv[4],"div_by");
        Q.div_by(div_by);
        std::cerr << "Q/" << div_by.value() << "=" << Q << std::endl;
    }

    std::cerr << "Arrangements: " << std::endl;
    for(size_t i=0;i<=10;++i)
    {
        std::cerr << i << ":";
        for(size_t j=0;j<=i;++j)
        {
            const mpl::pfd a = mpl::pfq::A(i,j);
            std::cerr << " " << a.value();
        }
        std::cerr << std::endl;
    }

    std::cerr << "Combinations: " << std::endl;
    for(size_t i=0;i<=10;++i)
    {
        std::cerr << i << ":";
        for(size_t j=0;j<=i;++j)
        {
            const mpl::pfd a = mpl::pfq::C(i,j);
            std::cerr << " " << a.value();
        }
        std::cerr << std::endl;
    }




}
Y_UTEST_DONE()

