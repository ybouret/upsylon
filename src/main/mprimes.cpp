#include "y/program.hpp"
#include "y/mpl/natural.hpp"
#include "y/ios/orstream.hpp"
#include "y/ios/osstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/convert.hpp"
#include "y/string/io.hpp"
#include <cmath>

using namespace upsylon;

Y_PROGRAM_START()
{
    size_t max_bytes = 1000;
    if(argc>1)
    {
        max_bytes = string_convert::to<size_t>(argv[1],"max_bytes");
    }
    
    MPN &mgr = MPN::instance();
    
    mgr.initProbe();
    
    size_t l    = 0;
    size_t iter = 0;
    while( (l=mgr.recordLength()) < max_bytes )
    {
        mgr.createPrimes(1,MPN::CreateFast);
        std::cerr << '.';
        if( 0== ((++iter)&63) )
        {
            std::cerr << '[' << mgr.plist.back().p << ']' << std::endl;
        }
    }
    std::cerr << "->[" << mgr.plist.back().p << "]" << std::endl;
    std::cerr << "bytes    = " << l << "/" << max_bytes << std::endl;
    std::cerr << "#primes  = " << mgr.plist.size() << std::endl;
    std::cerr << "@probe   = " << mgr.probe        << std::endl;
    
    {
        ios::orstream fp("mprimes.bin");
        if( mgr.recordPrimes(fp) != l )
        {
            throw exception("recordPrimes length mismatch");
        }
    }
    
    if(false)
    {
        const digest md0 = mgr.md();
        std::cerr << "md0=" << md0 << std::endl;
        const size_t cr = mgr.plist.size() - 2;
        mgr.initProbe();
        mgr.createPrimes(cr,MPN::CreateSafe);
        const digest md1 = mgr.md();
        std::cerr << "md1=" << md1 << std::endl;
        mgr.initProbe();
        mgr.createPrimes(cr,MPN::CreateFast);
        const digest md2 = mgr.md();
        std::cerr << "md2=" << md2 << std::endl;
    }
}
Y_PROGRAM_END()

