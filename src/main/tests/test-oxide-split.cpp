#include "y/oxide/field3d.hpp"
#include "y/oxide/field/io.hpp"
#include "y/oxide/optimal-mapping.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>


using namespace upsylon;
using namespace Oxide;


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
            Coord::LD(org,1);
            Coord::LD(top,cores);
            multi_loop<Coord1D,COORD> loop( org, top );

            for( loop.start(); loop.active(); loop.next() )
            {
                if( cores == size_t( Coord::Product(loop.value) ) )
                {
                    ++count;
                }
            }

        }
        return count;
    }


    template <typename COORD>
    static inline void split_layout( const Layout<COORD> &L, const size_t nmax)
    {
        std::cerr << "In " << L.Dimensions << "D" << std::endl;
        std::cerr << "L=" << L << std::endl;
        vector<COORD> mappings(nmax,as_capacity);

        for(size_t cores=1;cores<=nmax;++cores)
        {
            // let's take a given #cores
            // and build the possible mappings for this layoyt
            std::cerr << "\tcores=" << cores << std::endl;
            L.buildMappings(mappings,cores);
            std::cerr << "\tmappings=" << mappings << std::endl;
            if(mappings.size()<=0)
            {
                std::cerr << "\t\tfailure" << std::endl;
                continue;
            }

            // for each valid mapping, compute associated partition
            const size_t npart = mappings.size();
            std::cerr << "\ttesting partitions: " << std::endl;
            vector< Layout<COORD> > partition;
            for(size_t i=1;i<=npart;++i)
            {
                const COORD &mapping = mappings[i];
                std::cerr << "\t\t" << mapping << " [[";
                L.buildPartition(partition,mapping);
                Y_ASSERT(cores==partition.size());
                for(size_t j=1;j<=partition.size();++j)
                {
                    std::cerr << " " << partition[j];
                }
                std::cerr << " ]] maxItems=" << L.getPartitionMaxItems(mapping) << std::endl;

            }

            const COORD opt = OptimalMapping::Find(L,cores);
            std::cerr << "Found to be " << opt << std::endl;
        }
    }

}

#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(oxide_split)
{

    size_t nmax=2; if(argc>1) nmax = string_convert::to<size_t>(argv[1],"nmax");
    
    const Coord3D org(1,1,1);
    Coord3D       top(10,20,30);
    if(argc>2)    top = Coord::Parse<Coord3D>(argv[2]);


    {
        Layout1D L(org.x,top.x);
        split_layout(L,nmax);;
    }

    {
        Layout2D L(org.xy(),top.xy());
        split_layout(L,nmax);
    }

    {
        Layout3D L(org,top);
        split_layout(L,nmax);
    }





}
Y_UTEST_DONE()



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
