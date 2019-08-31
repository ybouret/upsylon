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
        std::cerr << "\tcores=" << cores << std::endl;
        L.buildMappings(mappings,cores);
        std::cerr << "\tmappings=" << mappings << std::endl;
        if(mappings.size()<=0)
        {
            std::cerr << "\t\tfailure" << std::endl;
            continue;
        }
        const size_t npart = mappings.size();
        std::cerr << "\ttesting partitions: " << std::endl;
        vector< Layout<COORD> > partition;
        for(size_t i=1;i<=npart;++i)
        {
            const COORD &mapping = mappings[i];
            std::cerr << "\t\t" << mapping << std::endl;
            L.buildPartition(partition,mapping);
            std::cerr << "\t\t"; Y_CHECK(cores==partition.size());
            for(size_t j=1;j<=partition.size();++j)
            {
                std::cerr << "\t\t\t" << partition[j] << std::endl;
            }

        }
    }
}

#include "y/string/convert.hpp"

Y_UTEST(oxide_split)
{

    size_t nmax=2; if(argc>1) nmax = string_convert::to<size_t>(argv[1],"nmax");
    size_t w = 10; if(argc>2) w    = string_convert::to<size_t>(argv[2],"w");
    size_t h = 20; if(argc>3) h    = string_convert::to<size_t>(argv[3],"h");
    size_t H = 30; if(argc>4) H    = string_convert::to<size_t>(argv[4],"H");



    {
        Layout1D L(1,w);
        split_layout(L,nmax);;
    }

    {
        Layout2D L(Coord2D(1,1),Coord2D(w,h));
        split_layout(L,nmax);
    }

    {
        Layout3D L(Coord3D(1,1,1),Coord3D(w,h,H));
        split_layout(L,nmax);
    }





}
Y_UTEST_DONE()

