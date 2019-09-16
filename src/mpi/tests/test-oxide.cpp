#include "y/oxide/mpi/domain.hpp"
#include "y/oxide/mpi/realm.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/imstream.hpp"
#include "y/oxide/field/io.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Oxide;

namespace
{
    template <typename T>
    void fill( FieldOf<T> &F )
    {
        for(size_t i=F.localObjects/2;i>0;--i)
        {
            const T tmp = support::get<T>();
            F.entry[ alea.range<Coord1D>(0,F.localObjects-1) ] = tmp;
        }
    }

    static inline Coord1D LabelOf( Coord1D rank )
    {
        return 1+rank*rank;
    }

    template <typename FIELD,typename COORD>
    void CheckValueOf( const FIELD &F, const Layout<COORD> &L, Coord1D V )
    {
        typename Layout<COORD>::Loop loop(L.lower,L.upper);
        for( loop.start(); loop.valid(); loop.next() )
        {
            if(V!=F(loop.value)) throw exception("Mismatch Value %ld/%ld",long(F(loop.value)),long(V));
        }
    }

}

template <typename COORD>
void make_for(mpi  &MPI,
              const Layout<COORD> &full )
{
    
    typedef typename __Field<COORD,double>::Type   dField;
    typedef typename __Field<COORD,mpn>::Type      nField;
    typedef typename __Field<COORD,Coord1D>::Type  iField;
    
    vector<COORD> mappings;
    full.buildMappings(mappings, MPI.size);
    
    MPI.print0(stderr, "\n");
    MPI.print(stderr, "in %uD on %s\n", unsigned( Layout<COORD>::Dimensions ), *MPI.processorName);
    if(MPI.isHead)
    {
        fflush(stderr);
        std::cerr << "Full       : " << full     << std::endl;
        std::cerr << "|_Mappings : " << mappings << std::endl;
        std::cerr.flush();
    }

    ActiveFields fields;




    for(size_t m=1;m<=mappings.size();++m)
    {
        const COORD                 &mapping = mappings[m];
        Realm<COORD>               *parent  = 0;
        auto_ptr< Realm<COORD> >    guard   = 0;
        if(MPI.isHead)
        {
            fflush(stderr);
            std::cerr << "|_: using ";
            Coord::Disp(std::cerr,mappings[m],3) << " [";
            std::cerr.flush();
            //! will share the same parent, discard boundaries and ghosts zone
            parent = new Realm<COORD>(MPI,full,mapping,Coord::Zero<COORD>(),0);
            guard  = parent;
            parent->template create<dField>( "Fd" );
            parent->template create<nField>( "Fn" );
            parent->template create<iField>( "Fi" );

        }
        //MPI.print(stderr,"|_parent: %d\n", int( parent.is_valid() ) );
        

        typename Layout<COORD>::Loop pbc(Coord::Zero<COORD>(),Coord::Ones<COORD>());
        for(pbc.start(); pbc.valid(); pbc.next())
        {

            size_t ghostsZone=1;

            MPI.Barrier();
            Parallel<COORD> ctx(MPI,full,pbc.value);
            Domain<COORD>   W(MPI, full, mapping, pbc.value,ghostsZone);
            
            if( ctx.optimal == mapping )
            {
                MPI.print0(stderr,"*");
            }
            else
            {
                MPI.print0(stderr,".");
            }
            MPI.flush(stderr);

            Y_ASSERT(W.rank==MPI.rank);
            
            dField &Fd = W. template create<dField>( "Fd" );
            iField &Fi = W. template create<iField>( "Fi" );

            fields(W);
            Y_ASSERT(fields.getCommMode()==comm_constant_size);


            // prepare
            fill(Fd);
            const Coord1D label =LabelOf(MPI.rank);
            IO::LD(Fi,W.outer,-label);
            IO::LD(Fi,W.inner, label);

            // check local labels: outer ghosts have now inner values
            for( const typename Workspace<COORD>::gNode *node = W.localGhosts.head; node; node=node->next)
            {
                CheckValueOf<iField,COORD>( Fi, node->gio.forward->outer, -label );
                CheckValueOf<iField,COORD>( Fi, node->gio.forward->inner, label );
                CheckValueOf<iField,COORD>( Fi, node->gio.reverse->outer, -label );
                CheckValueOf<iField,COORD>( Fi, node->gio.reverse->inner, label);
            }
            // exchange
            W.localExchange(fields);

            // check local labels: outer ghosts have now inner values
            for( const typename Workspace<COORD>::gNode *node = W.localGhosts.head; node; node=node->next)
            {
                CheckValueOf<iField,COORD>( Fi, node->gio.forward->outer, label );
                CheckValueOf<iField,COORD>( Fi, node->gio.forward->inner, label );
                CheckValueOf<iField,COORD>( Fi, node->gio.reverse->outer, label );
                CheckValueOf<iField,COORD>( Fi, node->gio.reverse->inner, label);
            }

            // reset values
            IO::LD(Fi,W.outer,-label);
            IO::LD(Fi,W.inner, label);
            
            // and now, let's go asynchronous
            W.asyncExchange(fields);
            
            // inner value must be unchanged
            CheckValueOf<iField,COORD>(Fi, W.inner, label);
            
            // aync ghosts must have their own values
            for(size_t k=0;k<W.Orientations;++k)
            {
                typename Workspace<COORD>::Peer peer = W.getAsyncPeer(Conn::Forward,k);
                if(peer)
                {
                    CheckValueOf<iField,COORD>( Fi, peer->outer, LabelOf(peer->rank) );
                }
                peer = W.getAsyncPeer(Conn::Reverse,k);
                if(peer)
                {
                    CheckValueOf<iField,COORD>( Fi, peer->outer, LabelOf(peer->rank) );
                }
            }

            // simulate some computation
            MPI.Barrier();

            for(size_t cycle=0;cycle<8;++cycle)
            {
                fill( Fd );
                fill( Fi );
                W.localExchange(fields);
                W.asyncExchange(fields);
            }


            // then uses flexible exchanges
            nField &Fn = W. template create<nField>( "Fn" );
            fields(W);

            for(size_t cycle=0;cycle<8;++cycle)
            {
                fill(Fn);
                fill(Fi);
                fill(Fd);
                W.localExchange(fields);
                W.asyncExchange(fields);
            }

            MPI.print0(stderr,"^");
            MPI.flush(stderr);

            MPI.Barrier();

            // Gathering
            MPI.print0(stderr,"<");
            MPI.flush(stderr);
            IO::LD(Fi,W.outer, -1);
            IO::LD(Fi,W.inner,W.rank);
            if(parent)
            {
                iField &gFi = parent->template as<iField>("Fi");
                IO::LD(gFi,parent->outer, -1);
            }
            Realm<COORD>::Gather( MPI,parent, "Fi", W);

            if(parent)
            {
                iField &gFi = parent->template as<iField>("Fi");
                for(size_t i=0;i<parent->partition.size();++i)
                {
                    CheckValueOf(gFi,parent->partition[i],i);
                }
                MPI.print0(stderr,"$");
            }
            MPI.print0(stderr,"<");
            MPI.flush(stderr);
            Realm<COORD>::Gather( MPI,parent, "Fd", W);

            MPI.print0(stderr,"<");
            MPI.flush(stderr);
            Realm<COORD>::Gather( MPI,parent, "Fn", W);



            // Scattering
            if(parent)
            {
                iField &gFi = parent->template as<iField>("Fi");
                IO::LD(gFi,parent->outer, -1);
            }
            IO::LD(Fi,W.outer,W.rank);
            Realm<COORD>::Scatter(MPI,parent, "Fi", W);
            CheckValueOf(Fi,W.inner,-1);
            MPI.print0(stderr,"#");


            MPI.print0(stderr,">");
            MPI.flush(stderr);
            Realm<COORD>::Scatter(MPI,parent, "Fd", W);
            
            MPI.print0(stderr,">");
            MPI.flush(stderr);
            Realm<COORD>::Scatter(MPI,parent, "Fn", W);
            
        } MPI.print0(stderr,"]\n");
    }



}

Y_UTEST(oxide)
{
    //static const int tag = 7;
    Y_MPI(SINGLE);

    const Coord3D  lower(1,1,1);
    const Coord3D  org(1,1,1);
    const Coord3D  top(2,2,2);
    Layout3D::Loop loop(org,top);

    for( loop.start(); loop.valid(); loop.next() )
    {
        const Coord3D  upper = 8 * loop.value;
        const Layout1D full1D( lower.x, upper.x);
        const Layout2D full2D( lower.xy(), upper.xy());
        const Layout3D full3D(lower,upper);


        make_for(MPI,full1D);
        make_for(MPI,full2D);
        make_for(MPI,full3D);

    }



}
Y_UTEST_DONE()

