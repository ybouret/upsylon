
#include "y/oxide/workspaces.hpp"
#include "y/utest/run.hpp"
#include "y/memory/pooled.hpp"
#include "y/string/tokenizer.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"

#include "support.hpp"

using namespace upsylon;
using namespace Oxide;

namespace
{
    typedef vector<string,memory::pooled> strings;
    static inline bool isSep( const char C ) { return C == ','; }

    template <typename FIELD>
    void display_field( const FIELD &F )
    {
        std::cerr << F.name << " : " << F << std::endl;
    }

    template <typename FIELD> static inline
    void fill(FIELD &F)
    {
        for(size_t i=0;i<F.items/2;++i)
        {
            const typename FIELD::type tmp = support::get<typename FIELD::mutable_type>();

            F( F.rand(alea) ) = tmp;
        }
    }

    template <typename COORD>
    static inline void testWksp(char **argv)
    {

        typedef  typename __Field<COORD,double>::Type  dField;
        typedef  typename __Field<COORD,float>::Type   fField;
        typedef  typename __Field<COORD,string>::Type  sField;

        const COORD   length  = Coord::Parse<COORD>(  argv[1],"length");
        const COORD   pbc     = Coord::Parse<COORD>(  argv[2],"pbc");
        const Coord1D ng      = Coord::Parse<Coord1D>(argv[3],"ng");
        const COORD   mapping = Coord::Parse<COORD>(  argv[4],"mapping");

        COORD org(0); Coord::LD(org,1);
        const Layout<COORD> full(org,length);
        std::cerr << "full=" << full << std::endl;

        Workspaces<COORD> WS(full,mapping,pbc,ng);
        for(size_t i=0;i<WS.size();++i)
        {
            Workspace<COORD> &W = WS[i];
            std::cerr << "W[" << i << "]: " << W.inner << std::endl;
            dField &Fd = W.template create<dField>( "Fd" );
            fField &Ff = W.template create<fField>( "Ff" );
            sField &Fs = W.template create<sField>( "Fs" );

            fill(Fd);
            fill(Ff);
            fill(Fs);
        }

    }


}

Y_UTEST(oxide_mpi)
{
    // length pbc ng mapping
    if(argc<=4)
    {
        std::cerr << "Usage: " << program << " length pbc ng mapping" << std::endl;
        return 0;
    }


    strings      length(3,as_capacity);
    const size_t dimensions = tokenizer<char>::split(length, argv[1], isSep);


    switch(dimensions)
    {
        case 1: testWksp<Coord1D>(argv); break;
        case 2: testWksp<Coord2D>(argv); break;
        case 3: testWksp<Coord3D>(argv); break;
        default: break;
    }
}
Y_UTEST_DONE()
