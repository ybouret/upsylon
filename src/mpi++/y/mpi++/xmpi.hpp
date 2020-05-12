
//! \file

#ifndef Y_XMPI_INCLUDED
#define Y_XMPI_INCLUDED 1

#include "y/mpi++/mpi.hpp"
#include "y/ios/conveyor.hpp"
#include "y/ios/ovstream.hpp"

namespace upsylon {
    
    //! eXtended MPI functions
    struct XMPI
    {
        //! Sending a range
        template <typename ITERATOR> static inline
        void Send(mpi &MPI,const int dest, ITERATOR curr, size_t n)
        {
            MPI.SendSize(n,dest);
            while(n-- > 0)
            {
                MPI.Send(*curr,dest);
            }
        }
        
        //! Sending a sequence
        template <typename SEQUENCE> static inline
        void Send(mpi &MPI,const int dest, const SEQUENCE &seq)
        {
            Send(MPI,dest,seq.begin(),seq.size());
        }
        
        //! Receiving into a sequence
        template <typename SEQUENCE> static inline
        void Recv(mpi &MPI, const int from, SEQUENCE &seq )
        {
            size_t n = MPI.RecvSize(from);
            while(n-- > 0)
            {
                typename SEQUENCE::const_type rcv = MPI.Recv<typename SEQUENCE::mutable_type>(from);
                seq.push_back(rcv);
            }
        }
        
        
        typedef ios::ovstream Buffer; //!< alias
        
        static void vSend(mpi                  &MPI,
                          const Buffer         &sendBuffer,
                          const int             dest,
                          const comms::delivery delivery);
        
        static void vRecv(mpi                  &MPI,
                          Buffer               &recvBuffer,
                          const int             source,
                          const comms::delivery delivery);
        
        static void vSendRecv(mpi                  &MPI,
                              const Buffer         &sendBuffer,
                              const int             dest,
                              Buffer               &recvBuffer,
                              const int             source,
                              const comms::delivery delivery);
        
    };
    
}

#endif
