
//! \file

#ifndef Y_XMPI_INCLUDED
#define Y_XMPI_INCLUDED 1

#include "y/mpi++/mpi.hpp"
#include "y/ios/conveyor.hpp"
#include "y/ios/ovstream.hpp"

namespace upsylon {
    
    //--------------------------------------------------------------------------
    //
    //! eXtended MPI functions
    //
    //--------------------------------------------------------------------------
    struct XMPI
    {
        //______________________________________________________________________
        //
        //
        // Send/Recv helpers
        //
        //______________________________________________________________________
        
        
        //______________________________________________________________________
        //
        //! Sending a range
        //______________________________________________________________________
        template <typename ITERATOR> static inline
        void Send(const mpi &MPI,const int dest, ITERATOR curr, size_t n)
        {
            MPI.SendSize(n,dest);
            while(n-- > 0)
            {
                MPI.Send(*curr,dest);
            }
        }
        
        //______________________________________________________________________
        //
        //! Sending a sequence
        //______________________________________________________________________
        template <typename SEQUENCE> static inline
        void Send(const mpi &MPI,const int dest, const SEQUENCE &seq)
        {
            Send(MPI,dest,seq.begin(),seq.size());
        }
        
        //______________________________________________________________________
        //
        //! Receiving into a sequence
        //______________________________________________________________________
        template <typename SEQUENCE> static inline
        void Recv(const mpi &MPI, const int from, SEQUENCE &seq )
        {
            size_t n = MPI.RecvSize(from);
            while(n-- > 0)
            {
                typename SEQUENCE::const_type rcv = MPI.Recv<typename SEQUENCE::mutable_type>(from);
                seq.push_back(rcv);
            }
        }
        
        
        //______________________________________________________________________
        //
        //
        // variadic blocks
        //
        //______________________________________________________________________
        
        typedef ios::ovstream Block; //!< alias
        
        //______________________________________________________________________
        //
        //! Send with size information if delivery==flexible block size
        //______________________________________________________________________
        static void vSend(const mpi                  &MPI,
                          const Block                &sendBuffer,
                          const int                   dest,
                          const comms::shipping_style style);
        
        //______________________________________________________________________
        //
        //! Recv with size adjustment if delivery==flexible block size
        //______________________________________________________________________
        static void vRecv(const mpi                  &MPI,
                          Block                      &recvBuffer,
                          const int                   source,
                          const comms::shipping_style style);
        
        //______________________________________________________________________
        //
        //! Send.Recv with size adjustment if delivery==flexible block size
        //______________________________________________________________________
        static void vSendRecv(const mpi                  &MPI,
                              const Block                &sendBuffer,
                              const int                   dest,
                              Block                      &recvBuffer,
                              const int                   source,
                              const comms::shipping_style style);
        
    };
    
}

#endif
