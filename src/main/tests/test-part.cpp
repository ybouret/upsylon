#include "y/counting/part.hpp"
#include "y/utest/run.hpp"
#include "y/counting/comb.hpp"

#include <iomanip>


using namespace upsylon;

namespace {




}

#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(part)
{


    size_t nmax = 5;
    if( argc > 1 )
    {
        nmax = string_convert::to<size_t>(argv[1],"n");
    }

    for(size_t n=nmax;n<=nmax;++n)
    {
        integer_partition pb(n);
        const size_t       ns = pb.outcomes();
        std::cerr << "<" << n << "> can be summed in " << ns << " outcomes" << std::endl;

        pb.initialize();
        size_t count = 0;
        mpn    mpBell = 0;
        size_t szBell = 0;
        do
        {
            ++count;
            std::cerr << pb << " ->";
            const mpn nc = pb.configs( counting::with_mp );
            std::cerr << "+" << nc << std::endl;
            mpBell += nc;
            szBell += pb.configs(counting::with_sz);
        }
        while( pb.build_next() );
        Y_CHECK(ns==count);
        std::cerr << "#outcomes = " << ns     << std::endl;
        std::cerr << "#mpBell   = " << mpBell << std::endl;
        std::cerr << "#szBell   = " << szBell << std::endl;
    }




}
Y_UTEST_DONE()

Y_UTEST(bell)
{
    const char  *BellTab[] =
    {
        "1",
        "1",
        "2",
        "5",
        "15",
        "52",
        "203",
        "877",
        "4140",
        "21147",
        "115975",
        "678570",
        "4213597",
        "27644437",
        "190899322",
        "1382958545",
        "10480142147",
        "82864869804",
        "682076806159",
        "5832742205057",
        "51724158235372",
        "474869816156751",
        "4506715738447323",
        "44152005855084346",
        "445958869294805289",
        "4638590332229999353",
        "49631246523618756274"
    };

    for(size_t i=1;i<sizeof(BellTab)/sizeof(BellTab[0]);++i)
    {
        const mpn BellNumber = mpn::parse(BellTab[i]);
        (std::cerr << std::setw(3) << i << ' ' << BellNumber << std::endl).flush();
        const mpn mpBell     = integer_partition::Bell<mpn>(i);
        Y_ASSERT(BellNumber==mpBell);
    }
}
Y_UTEST_DONE()
