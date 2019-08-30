#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"
#include "y/os/rt-clock.hpp"

using namespace upsylon;

typedef matrix<uint64_t>   UMatrix;


namespace
{
    static inline void fillRan( mpi::vBytes &blk )
    {
        for(size_t i=blk.size(); i>0; --i)
        {
            blk[i] = alea.full<uint8_t>();
        }
    }

    static inline
    size_t Star(mpi          &MPI,
              const uint8_t ln2blockSize,
              UMatrix      &tsnd,
              UMatrix      &trcv,
              UMatrix      &txch,
              const size_t  cycles)
    {
        const size_t blockSize = (1<<ln2blockSize);
        tsnd.ld(0);
        trcv.ld(0);
        txch.ld(0);

        if(!MPI.parallel)
        {
            return blockSize;
        }

        mpi::vBlock sndblk( blockSize, true);
        mpi::vBlock rcvblk( blockSize, true);
        fillRan(sndblk);

        //----------------------------------------------------------------------
        // testing all origins
        //----------------------------------------------------------------------
        for(int origin=0;origin<MPI.size;++origin)
        {
            const size_t ir = origin+1;
            MPI.print0(stderr, "<Star Topology: origin=%d, blockSize=%u>\n", origin, unsigned(blockSize) );
            MPI.Barrier();

            if(MPI.rank==origin)
            {
                //--------------------------------------------------------------
                // master node
                //--------------------------------------------------------------
                for(int r=0;r<MPI.size;++r)
                {
                    if(r==origin) continue;
                    const size_t ic = r+1;
                    for(size_t iter=0;iter<cycles;++iter)
                    {
                        // upload
                        MPI.vSend(comm_constant_size, sndblk, r, mpi::io_tag);
                        tsnd[ir][ic] += MPI.lastCommTicks;

                        // download
                        MPI.vRecv(comm_constant_size, rcvblk, r, mpi::io_tag);
                        trcv[ir][ic] += MPI.lastCommTicks;

                        // exchange
                        MPI.vSendRecv(comm_constant_size,
                                      sndblk, r, mpi::io_tag,
                                      rcvblk, r, mpi::io_tag);
                        txch[ir][ic] += MPI.lastCommTicks;
                    }
                }
            }
            else
            {
                //--------------------------------------------------------------
                // slave node
                //--------------------------------------------------------------
                {
                    for(size_t iter=0;iter<cycles;++iter)
                    {
                        // dowload
                        MPI.vRecv(comm_constant_size, rcvblk, origin, mpi::io_tag);

                        // upload
                        MPI.vSend(comm_constant_size,sndblk, origin, mpi::io_tag);

                        // exchange
                        MPI.vSendRecv(comm_constant_size,
                                      sndblk, origin, mpi::io_tag,
                                      rcvblk, origin, mpi::io_tag);
                    }
                }

            }
        }
        //----------------------------------------------------------------------
        // all origins have been tested
        // recomposing all matrices
        //----------------------------------------------------------------------
        //MPI.print0(stderr, "\treconstructing\n");
        for(int origin=0;origin<MPI.size;++origin)
        {
            const size_t ir = origin+1;
            if(MPI.rank==origin)
            {
                // store data from other nodes
                for(int r=0;r<MPI.size;++r)
                {
                    if(r==origin) continue;
                    MPI.SendAll(tsnd[ir], r, mpi::io_tag);
                    MPI.SendAll(trcv[ir], r, mpi::io_tag);
                    MPI.SendAll(txch[ir], r, mpi::io_tag);

                }
            }
            else
            {
                // query data from origin
                MPI.RecvAll(tsnd[ir], origin, mpi::io_tag);
                MPI.RecvAll(trcv[ir], origin, mpi::io_tag);
                MPI.RecvAll(txch[ir], origin, mpi::io_tag);
            }
        }

        //const size_t num_bytes = blockSize * cycles;
        //const double mega      = square_of( 1024.0 );



        if(MPI.isHead)
        {
            //std::cerr << "tsnd=" << tsnd << std::endl;
            //std::cerr << "trcv=" << trcv << std::endl;
            //std::cerr << "txch=" << txch << std::endl;

        }
        return blockSize;
    }

}


#include "y/ios/ocstream.hpp"

static inline double t2s( const uint64_t tmx, const unsigned nb )
{
    rt_clock     clk;
    return  nb / clk(tmx) / (1024.0*1024.0);
}

Y_UTEST(topology)
{
    Y_MPI(SINGLE);

    const unsigned n = MPI.size;
    UMatrix snd(n,n);
    UMatrix rcv(n,n);
    UMatrix xch(n,n);

    const string logfile = "topology.dat";

    if(MPI.isHead)
    {
        ios::ocstream::overwrite(logfile);
    }

    const size_t cycles = 32;
    for(size_t ln2=8;ln2<=20;++ln2)
    {

        const unsigned blockSize = Star(MPI,ln2, snd,rcv,xch, cycles);
        const unsigned num_bytes = blockSize * cycles;
        if(MPI.isHead)
        {
            ios::ocstream fp(logfile,true);
            fp("[blockSize=%u]\n",blockSize);
            for(unsigned r=0;r<n;++r)
            {
                fp("|_origin@%2u:\n", r);
                const unsigned ir=r+1;
                for(unsigned s=0;s<n;++s)
                {
                    if(s==r) continue;
                    const unsigned is = s+1;
                    fp(" | ");
                    fp("%2u->%2u: %8.2f",    r,s, t2s( snd[ir][is], num_bytes) );
                    fp(" | %2u<-%2u: %8.2f", r,s, t2s( rcv[ir][is], num_bytes) );
                    fp(" | %2u<->%2u: %8.2f", r,s, t2s( xch[ir][is], num_bytes) );
                    fp("\n");
                }
            }
            fp << '\n';
        }
    }



}
Y_UTEST_DONE()

