
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

        vector<Coord1D> indices;
        L.collect(indices,s);


        Field1D<double> F( "F1", L );
        send_block.free();

        if(MPI.isHead)
        {
            fill(F);
            F.save_only(indices, send_block, IO::SaveBlock<double> );
        }
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );
        MPI.print0(stderr, "Exchange\n" );
        if(MPI.isHead)
        {
            fill(F);
            F.save_only(indices, send_block, IO::SaveBlock<double> );
            for(int r=1;r<MPI.size;++r)
            {
                // first send, used packed
                Comm::Send(MPI,send_block,r,Comm::Packed);
                // second send
                Comm::Send(MPI,send_block,r,Comm::Static);
            }
        }
        else
        {
            Comm::Recv(MPI,recv_block,0,Comm::Packed);
            Comm::Recv(MPI,recv_block,0,Comm::Static);
            ios::imstream fp(recv_block);
            F.load_only(indices,fp,IO::LoadBlock<double>);
        }
        MPI.print(stderr, "send_block.size=%u | recv_block.size=%u\n", unsigned(send_block.size()), unsigned( recv_block.size() ) );

    }
    
    
}
Y_UTEST_DONE()

