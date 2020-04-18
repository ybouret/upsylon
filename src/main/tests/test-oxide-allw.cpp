
#include "y/oxide/workspace.hpp"
#include "y/utest/run.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
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

        typedef typename FieldFor<COORD,double>::Type            dField;
        typedef typename FieldFor<COORD,string>::Type            sField;
        typedef typename FieldFor<COORD,point2d<double> >::Type  p2Field;

        const size_t  ng = 1;
        ios::ovstream block( 1024*1024 );

        ActiveFields  active;

        std::cerr << "In " << full.Dimensions << "D" << std::endl;
        for(size_t size=1;size<=8;++size)
        {
            std::cerr << "#cores=" << size << ", full=" << full << std::endl;
            vector<COORD> mappings;
            full.buildMappings(mappings,size);
            for(size_t j=1;j<=mappings.size();++j)
            {
                std::cerr << "/mapping=" << mappings[j];
                typename Layout<COORD>::Loop pbc( Coord::Zero<COORD>(), Coord::Ones<COORD>() );
                for(pbc.boot(); pbc.good(); pbc.next())
                {
                    for(size_t rank=0;rank<size;++rank)
                    {

                        Workspace<COORD> W(full,mappings[j],rank,pbc.value,ng);

                        {
                            dField  &Fd = W.template create<dField>( "Fd" );
                            sField  &Fs = W.template create<sField>( "Fs" );
                            p2Field &F2 = W.template create<p2Field>( "F2" );
                            fill(Fd);
                            fill(Fs);
                            fill(F2);
                        }
                        active.free();
                        active(W,"Fd;F2"); Y_ASSERT(2==active.size());
                        Y_ASSERT( comms::constant    == active.getCommMode()  );
                        Y_ASSERT( 3*sizeof(double)   == active.getBlockSize() );

                        W.localExchange(active);
                        typename Workspace<COORD>::AsyncIO  aio;
                        for(size_t k=0;k<W.Orientations;++k)
                        {
                            W.asyncProlog(aio,active,Conn::Forward,k);
                            if(W.sendBlock.size()>0)
                            {
                                W.recvBlock.copy( W.sendBlock );
                            }
                            W.asyncEpilog(aio,active);

                            W.asyncProlog(aio,active,Conn::Reverse,k);
                            if(W.sendBlock.size()>0)
                            {
                                W.recvBlock.copy( W.sendBlock );
                            }
                            W.asyncEpilog(aio,active);
                        }

                    }
                }
            } std::cerr << std::endl;

        }


    }

}


Y_UTEST(oxide_allw)
{

    const Coord3D  lower(1,1,1);
    const Coord3D  org(0,0,0);
    const Coord3D  top(2,2,2);
    Layout3D::Loop loop(org,top);

    for( loop.boot(); loop.good(); loop.next() )
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

