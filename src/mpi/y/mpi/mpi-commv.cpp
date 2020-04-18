#include "y/mpi/mpi.hpp"


namespace upsylon
{
    void mpi::SendSize(const size_t     value,
                       const int        target,
                       const int        tag)
    {
        send_pack(value);
        Send(send_pack.ro(), send_pack.requested, MPI_BYTE, target, tag);
    }
    
    size_t mpi:: RecvSize(const int        source,
                          const int        tag)
    {
        Recv(recv_pack.get().rw(),recv_pack.requested, MPI_BYTE, source, tag);
        return recv_pack();
    }
    
    size_t mpi:: SendRecvSizes(const size_t value,
                               const int target, const int sendtag,
                               const int source, const int recvtag)
    {
        send_pack(value);
        SendRecv(send_pack.ro(),       send_pack.requested, MPI_BYTE, target, sendtag,
                 recv_pack.get().rw(), recv_pack.requested, MPI_BYTE, source, recvtag);
        return recv_pack();
    }
    
}


namespace upsylon
{
    void mpi:: vSend(const comms::manner mode,
                     const vBytes       &v,
                     const int           target,
                     const int           tag)
    {
        const size_t n = v.size();
        switch(mode)
        {
            case comms::variable: SendSize(v.size(),target,tag); /* FALLTHRU */
            case comms::constant: if(n>0) Send(*v,n, MPI_BYTE, target, tag); break;
        }
    }
    
    void mpi:: vRecv(const comms::manner  mode,
                     vBlock          &v,
                     const int        source,
                     const int        tag)
    {
        size_t n = v.size();
        switch(mode)
        {
            case comms::variable: n = RecvSize(source,tag); v.set_fast(n); /* FALLTHRU */
            case comms::constant: if(n>0) Recv(*v,n,MPI_BYTE,source,tag); break;
        }
    }
    
    void mpi:: vSendRecv(const comms::manner mode,
                         const vBytes   &sendBytes, const int target, const int sendtag,
                         vBlock         &recvBytes, const int source, const int recvtag)
    {
        const size_t ns = sendBytes.size();
        size_t       nr = recvBytes.size();
        
        switch (mode)
        {
            case comms::variable: recvBytes.set_fast( (nr = SendRecvSizes(ns, target, sendtag, source, recvtag) ) ); /* FALLTHRU */
            case comms::constant: SendRecv(*sendBytes, ns, MPI_BYTE, target, sendtag,
                                           *recvBytes, nr, MPI_BYTE, source, recvtag); break;
        }
    }
    
    
    
}
