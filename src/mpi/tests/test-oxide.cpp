#include "y/oxide/field/mpi.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/imstream.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Oxide;

template <typename T>
void fill( FieldOf<T> &F )
{
    for(size_t i=F.localObjects/2;i>0;--i)
    {
        const T tmp = support::get<T>();
        F.entry[ alea.range<Coord1D>(0,F.localObjects-1) ] = tmp;
    }
}

Y_UTEST(oxide)
{
    static const int tag = 7;
    Y_MPI(SINGLE);

    const Coord3D  lower(1,1,1);
    const Coord3D  org(1,1,1);
    const Coord3D  top(2,2,2);
    Layout3D::Loop loop(org,top);

    for( loop.start(); loop.valid(); loop.next() )
    {
        const Coord3D  upper = lower + 5 * loop.value;
        const Layout1D full1D( lower.x, upper.x);
        const Layout2D full2D( lower.xy(), upper.xy());
        const Layout3D full3D(lower,upper);


        ParallelContext<Coord1D> Par1D(MPI,full1D);
        ParallelContext<Coord2D> Par2D(MPI,full2D);
        ParallelContext<Coord3D> Par3D(MPI,full3D);

        if(MPI.isHead)
        {
            std::cerr << "Par1D mappings" << Par1D.mappings << " / " << Par1D.optimal << std::endl;
            std::cerr << "Par2D mappings" << Par2D.mappings << " / " << Par2D.optimal << std::endl;
            std::cerr << "Par3D mappings" << Par3D.mappings << " / " << Par3D.optimal << std::endl;

        }


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

