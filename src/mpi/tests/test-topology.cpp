#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"
#include "y/os/rt-clock.hpp"

using namespace upsylon;

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
    void Star(mpi          &MPI,
              const uint8_t ln2blockSize,
              const size_t  cycles)
    {
        rt_clock clk;

        if(!MPI.parallel)
        {
            return;
        }

        const size_t blockSize = (1<<ln2blockSize);
        mpi::vBlock sndblk( blockSize, true);
        mpi::vBlock rcvblk( blockSize, true);
        fillRan(sndblk);

        const size_t n = MPI.size;
        matrix<uint64_t> tsnd(n,n);
        matrix<uint64_t> trcv(n,n);
        matrix<uint64_t> txch(n,n);

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
                        MPI.vSend(mpi::Static, sndblk, r, mpi::io_tag);
                        tsnd[ir][ic] += MPI.lastCommTicks;

                        // download
                        MPI.vRecv(mpi::Static, rcvblk, r, mpi::io_tag);
                        trcv[ir][ic] += MPI.lastCommTicks;

                        // exchange
                        MPI.vSendRecv(mpi::Static,
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
                        MPI.vRecv(mpi::Static, rcvblk, origin, mpi::io_tag);

                        // upload
                        MPI.vSend(mpi::Static,sndblk, origin, mpi::io_tag);

                        // exchange
                        MPI.vSendRecv(mpi::Static,
                                      sndblk, origin, mpi::io_tag,
                                      rcvblk, origin, mpi::io_tag);
                    }
                }

            }
        }
        //----------------------------------------------------------------------
        // all origins have been tested
        //----------------------------------------------------------------------
        if(MPI.isHead)
        {
            std::cerr << "tsnd=" << tsnd << std::endl;
            std::cerr << "trcv=" << trcv << std::endl;
            std::cerr << "txch=" << txch << std::endl;
        }
        MPI.Barrier();
    }

}

Y_UTEST(topology)
{
    Y_MPI(SINGLE);

    Star(MPI,12,8);

}
Y_UTEST_DONE()

