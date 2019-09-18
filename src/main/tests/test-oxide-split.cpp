#include "y/oxide/field3d.hpp"
#include "y/sequence/vector.hpp"
#include "y/utest/run.hpp"
#include "y/oxide/layouts.hpp"

#include "support.hpp"
#include <typeinfo>
#include "y/string/tokenizer.hpp"
#include "y/oxide/field/divide.hpp"

using namespace upsylon;
using namespace Oxide;


namespace
{
    typedef vector<string> strings;
    
   

    
    template <typename COORD>
    void doSplit( const char *args , const Coord1D cores )
    {
        const COORD ini = Coord::Zero<COORD>();
        COORD       org = Coord::Ones<COORD>();
        const COORD top   = Coord::Parse<COORD>(args,"length");

        typename Layout<COORD>::Loop pbc(ini,org);
        const Layout<COORD>          full(org,top);
        std::cerr << "full=" << full << std::endl;

        vector<COORD> mappings;
        for(pbc.start(); pbc.valid(); pbc.next() )
        {
            std::cerr << "|_pbc=" << pbc.value << std::endl;
            const COORD opt =  Divide::Find(full, cores, pbc.value, &mappings);
            std::cerr << "|_opt=" << opt << " <= " << mappings << std::endl;
        }
       
    }

}

#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(oxide_split)
{
    std::cerr << "sizeof(Score)=" << sizeof(Divide::Scores) << std::endl;
    
    // length pbc ng mapping
    if(argc<=2)
    {
        std::cerr << "Usage: " << program << " length cores" << std::endl;
        return 0;
    }


    strings       length(3,as_capacity);
    const size_t  dimensions = tokenizer<char>::split_with(length, argv[1], ',');
    const Coord1D cores      = string_convert::to<size_t>(argv[2],"cores"); if(cores<=0) throw exception("cores<=0");


    
    switch( dimensions )
    {
        case 1: doSplit<Coord1D>( argv[1], cores );break;
        case 2: doSplit<Coord2D>( argv[1], cores );break;
        case 3: doSplit<Coord3D>( argv[1], cores );break;
        default:
            throw exception("Invalid dimensions");
    }
    


}
Y_UTEST_DONE()

namespace
{
    template <typename COORD> static inline
    size_t maxSplits( const size_t cores )
    {

        size_t count = 0;

        if(cores>0)
        {
            COORD  org(0);
            COORD  top(0);
            Coord::Ld(org,1);
            Coord::Ld(top,cores);
            mloop<Coord1D,COORD> loop( org, top );

            for( loop.start(); loop.valid(); loop.next() )
            {
                if( cores == size_t( Coord::Product(loop.value) ) )
                {
                    ++count;
                }
            }

        }
        return count;
    }
}

Y_UTEST(oxide_max_splits)
{
    std::cerr << "Computing MaxSplits" << std::endl;
    ios::ocstream fp("splits.dat");
    for(unsigned long cores=0;cores<=128;++cores)
    {
        const unsigned long m1 = maxSplits<Coord1D>(cores);
        const unsigned long m2 = maxSplits<Coord2D>(cores);
        const unsigned long m3 = maxSplits<Coord3D>(cores);
        fp("%lu %lu %lu %lu\n",cores,m1,m2,m3);

    }
}
Y_UTEST_DONE()
