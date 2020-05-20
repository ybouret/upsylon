

#include "y/mpi++/xmpi.hpp"

namespace upsylon {
    
    void XMPI:: vSend(const mpi                  &MPI,
                      const Block                &sendBuffer,
                      const int                   dest,
                      const comms::shipping_style style)
    {
        const size_t size = sendBuffer.size();
        switch(style)
        {

            case comms::flexible_block_size: MPI.SendSize(size,dest); /* FALLTHRU */
            case comms::computed_block_size: if(size>0) MPI.Send(*sendBuffer,size,dest); break;
        }
    }
    
    void XMPI:: vRecv(const mpi                  &MPI,
                      Block                      &recvBuffer,
                      const int                   source,
                      const comms::shipping_style style)
    {
        size_t size = recvBuffer.size();
        switch(style)
        {
            case comms::flexible_block_size: recvBuffer.adjust( size = MPI.RecvSize(source), 0); /* FALLTHRU */
            case comms::computed_block_size: assert(recvBuffer.size()==size); if(size>0) MPI.Recv(*recvBuffer,size,source); break;
                
        }
    }
    
    void XMPI:: vSendRecv(const mpi                  &MPI,
                          const Block                &sendBuffer,
                          const int                   dest,
                          Block                      &recvBuffer,
                          const int                   source,
                          const comms::shipping_style style)
    {
        const size_t sendSize = sendBuffer.size();
        size_t       recvSize = recvBuffer.size();
        switch(style)
        {
            case comms::flexible_block_size: recvBuffer.adjust(recvSize = MPI.ExchSizes(sendSize,dest,source),0); /* FALLTHRU */
            case comms::computed_block_size: assert(recvBuffer.size()==recvSize);MPI.Exch(*sendBuffer,sendSize, dest, *recvBuffer, recvSize, source); break;
        }
    }
    
}

