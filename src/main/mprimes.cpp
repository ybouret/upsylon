#include "y/program.hpp"
#include "y/mpl/mpn.hpp"
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
    
    mgr.reset();

    size_t       bytes    = 0;
    size_t       iter     = 0;
    while( (bytes=mgr.serialize_length()) < max_bytes )
    {
        mgr.createPrimes(1,MPN::CreateFast);
        std::cerr << '.';
        if( 0== ((++iter)&31) )
        {
            std::cerr << '[' << mgr.plist.back().p << ']' << " | #=" << iter << ", bytes=" << bytes << std::endl;
        }
    }


    std::cerr << "->[" << mgr.plist.back().p << "]" << std::endl;
    std::cerr << "bytes    = " << bytes << "/" << max_bytes << std::endl;
    std::cerr << "#primes  = " << mgr.plist.size() << std::endl;
    std::cerr << "@probe   = " << mgr.probe        << std::endl;
    std::cerr << "checking.." << std::endl;
    for(mpn i=0;i<=mgr.plist.back().p;++i)
    {
        mpn np = i;
        if(!mgr.locateNextPrime(np))
        {
            throw exception("could not locate a predicted value!!!");
        }
    }

    {
        ios::orstream fp("mprimes.bin");
        if( mgr.serialize(fp) != bytes )
        {
            throw exception("serialize length mismatch");
        }
    }
    const digest md_original = mgr.md();
    std::cerr << "md_original=" << md_original << std::endl;

    {
        ios::icstream fp("mprimes.bin");
        mgr.reloadPrimes(fp);
        const digest md_reloaded = mgr.md();
        std::cerr << "md_reloaded=" << md_reloaded << std::endl;

       if(md_original!=md_reloaded)
       {
           throw exception("reloadPrimes signature mismatch");
       }
    }
    
    if(false)
    {
        const size_t cr = mgr.plist.size() - 2;
        mgr.reset();
        mgr.createPrimes(cr,MPN::CreateSafe);
        const digest md1 = mgr.md();
        std::cerr << "md1=" << md1 << std::endl;
        mgr.reset();
        mgr.createPrimes(cr,MPN::CreateFast);
        const digest md2 = mgr.md();
        std::cerr << "md2=" << md2 << std::endl;
    }
}
Y_PROGRAM_END()

