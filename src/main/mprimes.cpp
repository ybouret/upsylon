#include "y/program.hpp"
#include "y/mpl/mpn.hpp"
#include "y/ios/orstream.hpp"
#include "y/ios/osstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/convert.hpp"
#include "y/string/io.hpp"
#include "y/container/tuple.hpp"

#include <cmath>

using namespace upsylon;

#if 0
Y_TRIPLE_DECL(STANDARD,marker,const mpn,p,const size_t,bytes,const size_t,bits);

Y_TRIPLE_END();

static inline void check_markers(size_t           &old_bits,
                                 sequence<marker> &markers,
                                 const size_t      bytes)
{
    MPN &mgr = MPN::location();

    const size_t new_bits = mgr.plist.back().p.bits();
    if( new_bits > old_bits )
    {
        old_bits = new_bits;
        const marker mk( mgr.plist.back().p, bytes,new_bits);
        markers.push_back(mk);
    }
}
#endif


Y_PROGRAM_START()
{
    size_t max_bytes = 1000;
    if(argc>1)
    {
        max_bytes = string_convert::to<size_t>(argv[1],"max_bytes");
    }
    
    MPN &mgr = MPN::instance();
    
    mgr.reset();

    //list<marker> markers;
    //size_t       old_bits = 0;
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
        //check_markers(old_bits,markers,bytes);
    }
    //check_markers(old_bits,markers,bytes);


    std::cerr << "->[" << mgr.plist.back().p << "]" << std::endl;
    std::cerr << "bytes    = " << bytes << "/" << max_bytes << std::endl;
    std::cerr << "#primes  = " << mgr.plist.size() << std::endl;
    std::cerr << "@probe   = " << mgr.probe        << std::endl;

    //std::cerr << "markers=" << markers << std::endl;

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

