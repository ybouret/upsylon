#include "y/mpi/mpi.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"
#include "y/os/rt-clock.hpp"
#include "y/code/hr-ints.hpp"

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
              const size_t    ln2blockSize,
              UMatrix        &tsnd,
              UMatrix        &trcv,
              UMatrix        &txch,
              const size_t    cycles)
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

        //----------------------------------------------------------------------
        // testing all origins
        //----------------------------------------------------------------------
        for(int origin=0;origin<MPI.size;++origin)
        {
            const size_t irow = origin+1;
            MPI.print0(stderr, "<Star Topology: origin=%d, blockSize=%u>\n", origin, unsigned(blockSize) );
            MPI.Barrier();

            if(MPI.rank==origin)
            {
                //--------------------------------------------------------------
                // master node
                //--------------------------------------------------------------
                for(int rank=0;rank<MPI.size;++rank)
                {
                    if(rank==origin) continue;
                    const size_t icol = rank+1;
                    for(size_t iter=0;iter<cycles;++iter)
                    {
                        fillRan(sndblk);
                        // upload
                        MPI.vSend(comms::computed_block_size, sndblk, rank, mpi::io_tag);
                        tsnd[irow][icol] += MPI.lastCommTicks;

                        // download
                        MPI.vRecv(comms::computed_block_size, rcvblk, rank, mpi::io_tag);
                        trcv[irow][icol] += MPI.lastCommTicks;

                        // exchange
                        MPI.vSendRecv(comms::computed_block_size,
                                      sndblk, rank, mpi::io_tag,
                                      rcvblk, rank, mpi::io_tag);
                        txch[irow][icol] += MPI.lastCommTicks;
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
                        MPI.vRecv(comms::computed_block_size, rcvblk, origin, mpi::io_tag);

                        // upload
                        MPI.vSend(comms::computed_block_size,sndblk, origin, mpi::io_tag);

                        // exchange
                        MPI.vSendRecv(comms::computed_block_size,
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
            const size_t irow = origin+1;
            if(MPI.rank==origin)
            {
                // store data from other nodes
                for(int r=0;r<MPI.size;++r)
                {
                    if(r==origin) continue;
                    MPI.SendAll(tsnd[irow], r, mpi::io_tag);
                    MPI.SendAll(trcv[irow], r, mpi::io_tag);
                    MPI.SendAll(txch[irow], r, mpi::io_tag);

                }
            }
            else
            {
                // query data from origin
                MPI.RecvAll(tsnd[irow], origin, mpi::io_tag);
                MPI.RecvAll(trcv[irow], origin, mpi::io_tag);
                MPI.RecvAll(txch[irow], origin, mpi::io_tag);
            }
        }


        return blockSize;
    }

}


#include "y/ios/ocstream.hpp"


static inline human_readable t2hr(const uint64_t tmx, const unsigned nb )
{
    rt_clock      clk;
    const int64_t spd = int64_t(ceil( nb/ clk(tmx) ));
    return human_readable(spd);
}

static inline void print_to( const char *sym, ios::ostream &fp, const unsigned r, const unsigned s, const uint64_t tmx, const unsigned nb )
{
    const human_readable hr = t2hr(tmx,nb);
    fp(" | %2u%s%2u: %8.2f%c/s",r,sym,s, hr.value, hr.radix);
}

Y_UTEST(star)
{
    Y_MPI(SINGLE);

    const unsigned n = MPI.size;
    UMatrix snd(n,n);
    UMatrix rcv(n,n);
    UMatrix xch(n,n);

    const string logfile = "star.dat";

    if(MPI.isHead)
    {
        ios::ocstream::overwrite(logfile);
    }

    const size_t cycles = 32;
    for(size_t ln2=8;ln2<=20;++ln2)
    {

        const unsigned blockSize = Star(MPI,ln2,snd,rcv,xch,cycles);
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
                    print_to("->",fp,r,s, snd[ir][is], num_bytes);
                    print_to("<-",fp,r,s, rcv[ir][is], num_bytes);
                    print_to("->",fp,r,s, xch[ir][is], num_bytes);
                    fp("\n");
                }
            }
            fp << '\n';
        }
    }



}
Y_UTEST_DONE()

