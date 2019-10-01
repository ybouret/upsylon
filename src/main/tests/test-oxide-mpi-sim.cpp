
#include "y/oxide/workspaces.hpp"
#include "y/utest/run.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/memory/cblock.hpp"
#include "y/oxide/field/io.hpp"

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

    static inline Coord1D LabelOf( Coord1D rank )
    {
        return 1+rank*rank;
    }

    template <typename FIELD,typename COORD>
    void CheckValueOf( const FIELD &F, const Layout<COORD> &L, typename FIELD::const_type V )
    {
        typename Layout<COORD>::Loop loop(L.lower,L.upper);
        for( loop.start(); loop.valid(); loop.next() )
        {
            if(V!=F(loop.value)) throw exception("Mismatch Value %ld/%ld",long(F(loop.value)),long(V));
        }
    }

    template <typename COORD>
    void make_all( const Layout<COORD> &full )
    {

        typedef typename FieldFor<COORD,double>::Type   dField;
        typedef typename FieldFor<COORD,string>::Type   sField;
        typedef typename FieldFor<COORD,Coord1D>::Type  iField;

        const size_t  ng = 1;
        ios::ovstream block( 1024*1024 );

        std::cerr << "In " << full.Dimensions << "D" << std::endl;
        ActiveFields sources,targets;


        for(Coord1D size=1;size<=27;++size)
        {
            std::cerr << "#cores=" << size << ", full=" << full << std::endl;
            memory::cblock_of<typename Workspace<COORD>::AsyncIO > aioData( size * Workspace<COORD>::Orientations );
            typename Workspace<COORD>::AsyncIO *aios = aioData.data;

            vector<COORD> mappings;
            full.buildMappings(mappings,size);
            for(size_t j=1;j<=mappings.size();++j)
            {
                std::cerr << "/mapping=" << mappings[j];

                typename Layout<COORD>::Loop pbc(Coord::Zero<COORD>(),Coord::Ones<COORD>());
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
                    for(Coord1D rank=0;rank<size;++rank)
                    {
                        Workspace<COORD> &W = WS[rank];

                        dField &Fd = W.template create<dField>( "Fd" );
                        sField &Fs = W.template create<sField>( "Fs" );
                        iField &Fi = W.template create<iField>( "Fi" );
                        fill(Fd);
                        fill(Fs);

                        IO::LD(Fi,W.outer,-LabelOf(rank));
                        IO::LD(Fi,W.inner, LabelOf(rank));


                    }

                    //----------------------------------------------------------
                    //
                    // exchange local
                    //
                    //----------------------------------------------------------
                    for(Coord1D rank=0;rank<size;++rank)
                    {
                        Workspace<COORD> &W = WS[rank];
                        sources(W);
                        W.localExchange(sources);
                        iField &Fi = W.template as<iField>( "Fi" );

                        // check local labels: outer ghosts have now inner values
                        for( const typename Workspace<COORD>::gNode *node = W.localGhosts.head; node; node=node->next)
                        {
                            CheckValueOf<iField,COORD>( Fi, node->gio.forward->outer, LabelOf(rank) );
                            CheckValueOf<iField,COORD>( Fi, node->gio.forward->inner, LabelOf(rank) );
                            CheckValueOf<iField,COORD>( Fi, node->gio.reverse->outer, LabelOf(rank) );
                            CheckValueOf<iField,COORD>( Fi, node->gio.reverse->inner, LabelOf(rank) );
                        }

                        // and reset for next adventure
                        IO::LD(Fi,W.outer,-LabelOf(rank));
                        IO::LD(Fi,W.inner, LabelOf(rank));
                        std::cerr << "0";
                    }

                    //----------------------------------------------------------
                    //
                    // simulate waves
                    //
                    //----------------------------------------------------------

                    // send forward
                    for(Coord1D rank=0,op=0;rank<size;++rank)
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

                                iField &Fi = WT.template as<iField>( "Fi" );
                                CheckValueOf(Fi,peer->outer, LabelOf(rank) );
                                CheckValueOf(Fi,peer->inner, LabelOf(aio.send->rank) );

                            }
                        }
                        std::cerr << "+";
                    }


                    // send reverse
                    for(Coord1D rank=0,op=0;rank<size;++rank)
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
                                iField &Fi = WT.template as<iField>( "Fi" );
                                CheckValueOf(Fi,peer->outer, LabelOf(rank) );
                                CheckValueOf(Fi,peer->inner, LabelOf(aio.send->rank) );
                            }
                        }
                        std::cerr << "-";
                    }


                }

            } std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }



}

Y_UTEST(oxide_mpi)
{
    const Coord3D  lower(1,1,1);
    const Coord3D  org(1,1,1);
    const Coord3D  top(2,2,2);
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
