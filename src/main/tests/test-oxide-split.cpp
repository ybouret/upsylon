#include "y/oxide/field3d.hpp"
#include "y/oxide/field/io.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>


using namespace upsylon;
using namespace Oxide;


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
            std::cerr << "\t\t" << mapping << std::endl;
            const size_t maxItems = L.buildPartition(&partition,mapping);
            std::cerr << "\t\t"; Y_CHECK(cores==partition.size());
            for(size_t j=1;j<=partition.size();++j)
            {
                std::cerr << "\t\t\t" << partition[j] << std::endl;
            }
            std::cerr << "\t\t: maxItems=" << maxItems << std::endl;

        }
    }
}

#include "y/string/convert.hpp"

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

