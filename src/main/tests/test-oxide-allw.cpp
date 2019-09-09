
#include "y/oxide/workspace.hpp"
#include "y/utest/run.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/ovstream.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Oxide;


namespace
{

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
    void make_all( const Layout<COORD> &full )
    {

        typedef typename __Field<COORD,double>::Type dField;
        typedef typename __Field<COORD,string>::Type sField;

        const size_t ng = 1;
        std::cerr << "In " << full.Dimensions << "D" << std::endl;
        for(size_t size=1;size<=8;++size)
        {
            std::cerr << "#cores=" << size << ", full=" << full << std::endl;
            vector<COORD> mappings;
            full.buildMappings(mappings,size);
            for(size_t j=1;j<=mappings.size();++j)
            {
                std::cerr << "|_mapping=" << mappings[j] << std::endl;
                COORD pbc0(0); Coord::LD(pbc0,0);
                COORD pbc1(1); Coord::LD(pbc1,1);

                typename Layout<COORD>::Loop pbc(pbc0,pbc1);
                for(pbc.start(); pbc.valid(); pbc.next())
                {
                    for(size_t rank=0;rank<size;++rank)
                    {
                        Workspace<COORD> W(full,mappings[j],rank,pbc.value,ng);

                        dField &Fd = W.template create<dField>( "Fd" );
                        sField &Fs = W.template create<sField>( "Fs" );

                        fill(Fd);
                        fill(Fs);


                        W.localExchange(Fd);
                        W.localExchange(Fs);
                    }
                }
            }

        }


    }

}


Y_UTEST(oxide_allw)
{

    const Coord3D lower(1,1,1);
    const Coord3D org(0,0,0);
    const Coord3D top(2,2,2);
    Layout3D::Loop loop(org,top);

    for( loop.start(); loop.valid(); loop.next() )
    {
        const Coord3D  upper = lower + 4 * loop.value;
        const Layout1D full1D( lower.x, upper.x);
        const Layout2D full2D( lower.xy(), upper.xy());
        const Layout3D full3D(lower,upper);

        make_all( full1D );
        make_all( full2D );
        make_all( full3D );

    }


}
Y_UTEST_DONE()

