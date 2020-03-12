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
    std::cerr << "<" << n << "> can be sum in " << ns << " outcomes" << std::endl;

    pb.initialize();
    size_t count = 0;
    do
    {
        ++count;
        std::cerr << pb << std::endl;
      }
    while( pb.build_next() );
    Y_CHECK(ns==count);
 }
Y_UTEST_DONE()

