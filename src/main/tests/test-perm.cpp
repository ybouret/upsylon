
#include "y/counting/perm.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


Y_UTEST(perm)
{
    size_t n = 5;
    if(argc>1)  n = string_convert::to<size_t>(argv[1]);

    permutation perm(n);

    std::cerr << "#perm(" << n << ")=" << perm.count << std::endl;
    for( perm.start(); perm.valid(); perm.next() )
    {
        std::cerr << "\t" << perm << std::endl;
    }
    std::cerr << "#perm(" << n << ")=" << perm.count << std::endl;

}
Y_UTEST_DONE()

