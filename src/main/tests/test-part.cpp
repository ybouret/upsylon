#include "y/counting/part.hpp"
#include "y/utest/run.hpp"
 
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
    
    partition::builder pb(n);
    const size_t       parts = pb.partitions();
    std::cerr << "#parts=" << parts << std::endl;
    
    pb.initialize();
    size_t count = 0;
    size_t num   = 0;
    do
    {
        ++count;
        std::cerr << pb << std::endl;
        const size_t np = pb.permutations();
        std::cerr << "\t|_" << np << std::endl;
        num += np;
    }
    while( pb.build_next() );
    Y_CHECK(parts==count);
    std::cerr << "#possibilities: " << num << std::endl;

}
Y_UTEST_DONE()

