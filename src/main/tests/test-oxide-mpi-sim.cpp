
#include "y/oxide/workspaces.hpp"
#include "y/utest/run.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/memory/cblock.hpp"

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
        ActiveFields sources,targets;


        for(size_t size=1;size<=8;++size)
        {
            std::cerr << "#cores=" << size << ", full=" << full << std::endl;
            memory::cblock_of<typename Workspace<COORD>::AsyncIO > aioData( size * Workspace<COORD>::Orientations );
            typename Workspace<COORD>::AsyncIO *aios = aioData.data;

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
                    for(size_t rank=0;rank<size;++rank)
                    {
                        Workspace<COORD> &W = WS[rank];

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
                    for(size_t rank=0;rank<size;++rank)
                    {
                        Workspace<COORD> &W = WS[rank];
                        sources(W);
                        W.localExchange(sources);
                    }
                    std::cerr << "0";

                    //----------------------------------------------------------
                    //
                    // simulate waves
                    //
                    //----------------------------------------------------------

                    // send forward
                    for(size_t rank=0,op=0;rank<size;++rank)
                    {
                        Workspace<COORD> &W = WS[rank];
                        sources(W);
                        for(size_t k=0;k<W.Orientations;++k)
                        {
                            typename Workspace<COORD>::AsyncIO &aio = aios[op++];
                            W.asyncProlog(aio, sources, Conn::Forward,k);
                            if(aio.send)
                            {
                                Y_ASSERT(W.sendBlock.size()>0);
                                Y_ASSERT(aio.send->rank<Coord1D(size));
                                Y_ASSERT(aio.send->rank>=0);
                                Y_ASSERT(aio.send->rank!=Coord1D(rank));
                                Workspace<COORD> &WT = WS[aio.send->rank];
                                WT.recvBlock.copy(W.sendBlock);

                                // get matching ghost
                                typename Workspace<COORD>::Peer peer = WT.getAsyncPeer(Conn::Reverse, k);
                                Y_ASSERT(peer!=0);
                                Y_ASSERT(peer->rank==Coord1D(rank));
                                targets(WT);
                                WT.__asyncLoad(*peer,targets);
                            }
                        }
                    }
                    std::cerr << "+";


                    // send reverse
                    for(size_t rank=0,op=0;rank<size;++rank)
                    {
                        Workspace<COORD> &W = WS[rank];
                        sources(W);
                        for(size_t k=0;k<W.Orientations;++k)
                        {
                            typename Workspace<COORD>::AsyncIO &aio = aios[op++];
                            W.asyncProlog(aio, sources, Conn::Reverse,k);
                            if(aio.send)
                            {
                                Y_ASSERT(W.sendBlock.size()>0);
                                Y_ASSERT(aio.send->rank<Coord1D(size));
                                Y_ASSERT(aio.send->rank>=0);
                                Y_ASSERT(aio.send->rank!=Coord1D(rank));
                                Workspace<COORD> &WT = WS[aio.send->rank];
                                WT.recvBlock.copy(W.sendBlock);

                                // get matching ghost
                                typename Workspace<COORD>::Peer peer = WT.getAsyncPeer(Conn::Forward, k);
                                Y_ASSERT(peer!=0);
                                Y_ASSERT(peer->rank==Coord1D(rank));
                                targets(WT);
                                WT.__asyncLoad(*peer,targets);
                            }
                        }
                    }
                    std::cerr << "-";


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
