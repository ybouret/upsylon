
#include "y/oxide/field/mpi.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/imstream.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Oxide;

template <typename T>
void fill( Field<T> &F )
{
    for(size_t i=F.localObjects/2;i>0;--i)
    {
        const T tmp = support::get<T>();
        F.entry[ alea.range<Coord1D>(0,F.localObjects-1) ] = tmp;
    }
}

Y_UTEST(oxide)
{
    Y_MPI(SINGLE);
    IO::Block send_block(1024);
    IO::Block recv_block(1024);
    vector<Coord1D> indices;
    ios::plugin_raw<double> plg;

    MPI.print0(stderr,"-------- 1D --------\n");
    {
        MPI.print0(stderr, "Setup\n" );
        Coord1D  lo = CoordOps::Integer<Coord1D>(20,alea);
        Coord1D  up = CoordOps::Integer<Coord1D>(20,alea);
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
            (void) F.save_only(indices, send_block, plg.save );
        }
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );
        MPI.print0(stderr, "Exchange\n" );
        if(MPI.isHead)
        {
            fill(F);
            send_block.free();
            const size_t nw = F.save_only(indices, send_block, plg.save );
            Y_ASSERT(nw==send_block.size());
            for(int r=1;r<MPI.size;++r)
            {
                // first send, used packed
                Comm::Send(MPI,send_block,r,comm_variable_size);
                // second send
                Comm::Send(MPI,send_block,r,comm_constant_size);
            }
        }
        else
        {
            F.ld(0);
            Comm::Recv(MPI,recv_block,0,comm_variable_size);
            Comm::Recv(MPI,recv_block,0,comm_constant_size);
            const size_t nl = F.load_only(indices,recv_block,plg.load);
            Y_ASSERT(nl==recv_block.size());
        }
        
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );
        
        MPI.print0(stderr, "Send/Recv Init\n" );
        send_block.free();
        recv_block.free();

        fill(F);
        F.save_only(indices,send_block, plg.save );
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );

        MPI.print0(stderr, "Send/Recv Exec\n" );

        if(MPI.parallel)
        {
            if(MPI.isHead)
            {
                for(int r=1;r<MPI.size;++r)
                {
                    Comm::SendRecv(MPI, send_block, r, recv_block, r,comm_variable_size);
                    Comm::SendRecv(MPI, send_block, r, recv_block, r,comm_constant_size);
                }
            }
            else
            {
                Comm::SendRecv(MPI, send_block, 0, recv_block, 0, comm_variable_size);
                Comm::SendRecv(MPI, send_block, 0, recv_block, 0, comm_constant_size);
            }
        }
        else
        {
            Comm::SendRecv(MPI, send_block, 0, recv_block, 0,comm_variable_size);
            Comm::SendRecv(MPI, send_block, 0, recv_block, 0,comm_constant_size);
        }
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );
        F.load_only(indices,recv_block,plg.load);
    }
    
    

    MPI.print(stderr, "Ellaped: %gms\n", MPI.getCommMilliseconds() );

}
Y_UTEST_DONE()

