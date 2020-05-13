

#include "y/mpi++/xmpi.hpp"

namespace upsylon {
    
    void XMPI:: vSend(mpi                  &MPI,
                      const Block          &sendBuffer,
                      const int             dest,
                      const comms::delivery delivery)
    {
        const size_t size = sendBuffer.size();
        switch(delivery)
        {
                
            case comms::flexible_block_size: MPI.SendSize(size,dest); /* FALLTHRU */
            case comms::computed_block_size: if(size>0) MPI.Send(*sendBuffer,size,dest); break;
        }
    }
    
    void XMPI:: vRecv(mpi                  &MPI,
                      Block                &recvBuffer,
                      const int             source,
                      const comms::delivery delivery)
    {
        size_t size = recvBuffer.size();
        switch(delivery)
        {
            case comms::flexible_block_size: recvBuffer.adjust( size = MPI.RecvSize(source), 0); /* FALLTHRU */
            case comms::computed_block_size: if(size>0) MPI.Recv(*recvBuffer,size,source); break;
                
        }
    }
    
    void XMPI:: vSendRecv(mpi                  &MPI,
                          const Block         &sendBuffer,
                          const int             dest,
                          Block                &recvBuffer,
                          const int             source,
                          const comms::delivery delivery)
    {
        const size_t sendSize = sendBuffer.size();
        size_t       recvSize = recvBuffer.size();
        switch(delivery)
        {
            case comms::flexible_block_size: recvBuffer.adjust(recvSize = MPI.ExchSizes(sendSize,dest,source),0); /* FALLTHRU */
            case comms::computed_block_size: MPI.Exch(*sendBuffer,sendSize, dest, *recvBuffer, recvSize, source); break;
        }
    }
    
}

