#include "y/counting/symm-pair.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/type/point2d.hpp"

using namespace upsylon;


Y_UTEST(symm_pair)
{
    size_t n = 10;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }
    std::cerr << n << " -> " << symm_pair::compute(n,counting::with_sz) << std::endl;

    symm_pair loop(n);
    
    const point2d<size_t> &vtx = *(const point2d<size_t> *) & loop.lower;
    
    for(loop.boot();loop.good();loop.next())
    {
        std::cerr << "@" << loop.index << " : " << loop.lower << "<=" << loop.upper << " " << vtx << std::endl;
    }

}
Y_UTEST_DONE()
