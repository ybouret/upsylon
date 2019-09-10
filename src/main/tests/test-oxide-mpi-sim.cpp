
#include "y/oxide/workspaces.hpp"
#include "y/utest/run.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"

#include "support.hpp"

using namespace upsylon;
using namespace Oxide;

namespace
{

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
    void make_all( const Layout<COORD> &full )
    {

        typedef typename __Field<COORD,double>::Type dField;
        typedef typename __Field<COORD,string>::Type sField;

        const size_t  ng = 1;
        ios::ovstream block( 1024*1024 );

        std::cerr << "In " << full.Dimensions << "D" << std::endl;
        Fields::Selection pick;

        for(size_t size=1;size<=8;++size)
        {
            std::cerr << "#cores=" << size << ", full=" << full << std::endl;
            vector<COORD> mappings;
            full.buildMappings(mappings,size);
            for(size_t j=1;j<=mappings.size();++j)
            {
                std::cerr << "/mapping=" << mappings[j];
                COORD pbc0(0); Coord::LD(pbc0,0);
                COORD pbc1(1); Coord::LD(pbc1,1);

                typename Layout<COORD>::Loop pbc(pbc0,pbc1);
                for(pbc.start(); pbc.valid(); pbc.next())
                {
                    std::cerr << ".";

                    //----------------------------------------------------------
                    //
                    // create all workspaces
                    //
                    //----------------------------------------------------------
                    Workspaces<COORD> WS( full, mappings[j], pbc.value, ng );


                    //----------------------------------------------------------
                    //
                    // create some fields
                    //
                    //----------------------------------------------------------
                    for(size_t k=0;k<size;++k)
                    {
                        Workspace<COORD> &W = WS[k];

                        dField &Fd = W.template create<dField>( "Fd" );
                        sField &Fs = W.template create<sField>( "Fs" );

                        fill(Fd);
                        fill(Fs);

                    }

                    //----------------------------------------------------------
                    //
                    // exchange local
                    //
                    //----------------------------------------------------------
                    for(size_t k=0;k<size;++k)
                    {
                        Workspace<COORD> &W = WS[k];
                        pick(W);
                        W.localExchange(pick);
                    }
                    std::cerr << "0";

                    //----------------------------------------------------------
                    //
                    // simulate forward wave
                    //
                    //----------------------------------------------------------
                    for(size_t k=0;k<size;++k)
                    {
                        Workspace<COORD> &W = WS[k];
                        pick(W);

                        for(size_t ori=0;ori<W.Orientations;++ori)
                        {
                            const _Ghosts<COORD> *G = 0;
                            const size_t ns = W.asyncSave(Connectivity::Forward,ori,pick,G);
                            if(G&&ns)
                            {

                            }
                        }
                    }


                }

            } std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }



}

Y_UTEST(oxide_mpi)
{
    const Coord3D lower(1,1,1);
    const Coord3D org(1,1,1);
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
