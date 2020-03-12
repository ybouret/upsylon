#include "y/counting/part.hpp"
#include "y/utest/run.hpp"
#include "y/counting/comb.hpp"

using namespace upsylon;

namespace {




}

#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(part)
{
    size_t n = 5;
    if( argc > 1 )
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }
    
    integer_partition pb(n);
    const size_t       ns = pb.outcomes();
    //Y_CHECK(ns== integer_partition::outcomes(n) );
    std::cerr << "<" << n << "> can be summed in " << ns << " outcomes" << std::endl;

    pb.initialize();
    size_t count = 0;
    mpn    total = 0;
    size_t iBell = 0;
    do
    {
        ++count;
        std::cerr << pb << " ->";
        const mpn nc = pb.mp_configurations();
        std::cerr << "+" << nc << std::endl;
        total += nc;
        iBell += pb.configurations();
      }
    while( pb.build_next() );
    Y_CHECK(ns==count);
    std::cerr << "#outcomes= " << ns << std::endl;
    std::cerr << "#total   = " << total << std::endl;
    std::cerr << "#Bell    = " << iBell << std::endl;
}
Y_UTEST_DONE()

