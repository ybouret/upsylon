#include "y/oxide/field/mpi.hpp"
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
    ParallelContext<COORD> ctx(MPI,full);

    typedef typename __Field<COORD,double>::Type   dField;
    //typedef typename __Field<COORD,mpq>::Type      qField;
    typedef typename __Field<COORD,Coord1D>::Type  iField;

    if(MPI.isHead)
    {
        fflush(stderr);
        std::cerr << "Full       : " << full  << std::endl;
        std::cerr << "|_Mappings : " << ctx.mappings << std::endl;
        std::cerr << "|_Optimal  : " << ctx.optimal  << std::endl;
        std::cerr.flush();
    }

    ActiveFields fields;

    for(size_t m=1;m<=ctx.mappings.size();++m)
    {
        const COORD &mapping = ctx.mappings[m];
        if(MPI.isHead)
        {
            fflush(stderr);
            std::cerr << "|_: using " << ctx.mappings[m] << " <";
            std::cerr.flush();
        }
        COORD pbc0(0); Coord::LD(pbc0,0);
        COORD pbc1(1); Coord::LD(pbc1,1);

        typename Layout<COORD>::Loop pbc(pbc0,pbc1);
        for(pbc.start(); pbc.valid(); pbc.next())
        {
            MPI.print0(stderr,".");
            MPI.flush0(stderr);
            
            size_t ng=1;

            MPI.Barrier();
            ParallelWorkspace<COORD> W(MPI,
                                       full,
                                       mapping,
                                       pbc.value,ng);

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

            // and now, let's go...
            W.asyncExchange(fields);
            
            
        } MPI.print0(stderr,">\n");
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
        //make_for(MPI,full2D);
        // make_for(MPI,full3D);



    }


#if 0
    mpi::vBlock     send_block(1024);
    mpi::vBlock     recv_block(1024);
    vector<Coord1D> indices;

    MPI.print0(stderr,"-------- 1D --------\n");
    {
        MPI.print0(stderr, "Setup\n" );
        Coord1D  lo = Coord::Integer<Coord1D>(20,alea);
        Coord1D  up = Coord::Integer<Coord1D>(20,alea);
        MPI.Bcast(lo,0);
        MPI.Bcast(up,0);
        const Layout1D L(lo,up);
        MPI.print(stderr, "full: lo=%ld, hi=%ld\n", long(L.lower), long(L.upper) );
        lo = L.rand(alea);
        up = L.rand(alea);
        MPI.Bcast(lo,0);
        MPI.Bcast(up,0);
        const Layout1D s(lo,up);
        MPI.print(stderr, "sub:  lo=%ld, hi=%ld\n", long(s.lower), long(s.upper) );

        L.collect(indices,s);


        Field1D<double> F( "F1", L );
        send_block.free();

        if(MPI.isHead)
        {
            fill(F);
            (void) F.save(indices, send_block );
        }
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );
        MPI.print0(stderr, "Exchange\n" );
        if(MPI.isHead)
        {
            fill(F);
            send_block.free();
            const size_t nw = F.save(indices, send_block );
            Y_ASSERT(nw==send_block.size());
            for(int r=1;r<MPI.size;++r)
            {
                // first send, used packed
                MPI.vSend(comm_variable_size,send_block,r,tag);
                // second send
                MPI.vSend(comm_constant_size,send_block,r,tag);
            }
        }
        else
        {
            F.ld(0);
            MPI.vRecv(comm_variable_size, recv_block, 0, tag);
            MPI.vRecv(comm_constant_size, recv_block, 0, tag);
            ios::imstream inp(recv_block);
            const size_t nl = F.load(indices,inp);
            Y_ASSERT(nl==recv_block.size());
        }
        
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );
        
        MPI.print0(stderr, "Send/Recv Init\n" );
        send_block.free();
        recv_block.free();

        fill(F);
        F.save(indices,send_block);
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );

        MPI.print0(stderr, "Send/Recv Exec\n" );

        if(MPI.parallel)
        {
            if(MPI.isHead)
            {
                for(int r=1;r<MPI.size;++r)
                {
                    MPI.vSendRecv(comm_variable_size, send_block, r, tag, recv_block, r, tag);
                    MPI.vSendRecv(comm_constant_size, send_block, r, tag, recv_block, r, tag);
                }
            }
            else
            {
                MPI.vSendRecv(comm_variable_size, send_block, 0, tag, recv_block, 0, tag);
                MPI.vSendRecv(comm_constant_size, send_block, 0, tag, recv_block, 0, tag);
            }
        }
        else
        {
            MPI.vSendRecv(comm_variable_size, send_block, 0, tag, recv_block, 0, tag);
            MPI.vSendRecv(comm_constant_size, send_block, 0, tag, recv_block, 0, tag);
        }
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );
        ios::imstream inp(recv_block);
        F.load(indices,inp);
    }
    
    

    MPI.print(stderr, "Ellaped: %gms\n", MPI.getCommMilliseconds() );
#endif

}
Y_UTEST_DONE()

