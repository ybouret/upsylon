
#include "y/mpi++/mpi.hpp"

namespace upsylon
{

    namespace
    {
        static const char fn[] = "mpi::sequential: ";
        static const char SYN = 1;
        static const char ACK = 2;
    }

    void mpi:: flush( const ios::cstdout_t & ) const throw()
    {
        if(stdout) fflush(stdout);
        try { std::cout.flush(); } catch(...) {}
    }

    void mpi:: flush( const ios::cstderr_t & ) const throw()
    {
        if(stderr) fflush(stderr);
        try { std::cerr.flush(); } catch(...) {}
    }


    void mpi::recvACK(const int r) const
    {
        if(ACK!=Recv<char>(r)) throw upsylon::exception("%sno ACK from rank=%d",fn,r);
    }
    
    void mpi::recvSYN() const
    {
        if( SYN != Recv<char>(0) ) throw upsylon::exception("%sno SYN from master node",fn);
    }
    
    void mpi::sendACK() const
    {
         Send(ACK,0);
    }
    
    void mpi::sendSYN(const int r) const
    {
        Send(SYN,r);
    }
    
    void mpi:: sequential(sequentialProc proc, void *args) const
    {
        assert(proc);
        
        if(head)
        {
            proc(*this,args);
            for(int r=1;r<size;++r)
            {
                sendSYN(r);
                recvACK(r);
            }
        }
        else
        {
            recvSYN();
            proc(*this,args);
            sendACK();
        }
    }
    
   
}

