//! \file

#ifndef Y_SPADE_MPI_INCLUDED
#define Y_SPADE_MPI_INCLUDED 1

#include "y/mpi++/xmpi.hpp"
#include "y/spade/workspace.hpp"

namespace upsylon {

    namespace Spade {

        
       
        
        
        //! handle multiple transfers
        class Synchronize : public Transfer
        {
        public:
            //! setup
            inline explicit Synchronize(const mpi                   &_,
                                        const comms::infrastructure  I) :
            Transfer(I),
            MPI(_)
            {
            }

            //! cleanup
            inline virtual ~Synchronize() throw() {}
            
            const mpi &MPI; //!< MPI instance

            //! forward waves, after asyncSetup was called
            /**
             send to forward, recv in reverse
             */
            // TODO: use GENERIC FIELD
            template <typename COORD>
            void forward(addressable<_Field>   &fields,
                         const Fragment<COORD> &fragment,
                         IOBlocks              &blocks)
            {
                assert(blocks.size() == 2*Fragment<COORD>::Levels);
                
                size_t iForward=0;
                size_t iTwoWays=0;
                size_t iReverse=0;
                
                for(unsigned i=0,j=0;i<Fragment<COORD>::Levels;++i)
                {
                    IOBlock                               &send  = blocks[j++];
                    IOBlock                               &recv  = blocks[j++];
                    const typename Topology<COORD>::Links &links = fragment[i];
                    
                    send.free();
                    switch(links.connect)
                    {
                        case Connect::AsyncTwoWays: {
                            const AsyncTwoWaysSwaps<COORD> &xch = fragment.asyncTwoWays[iTwoWays++];
                            const Ghosts                   &fwd = *(xch.forward);
                            const Ghosts                   &rev = *(xch.reverse);
                            
                           
                            asyncSave(send,fields,fwd.innerGhost);
                            asyncMake(recv,rev.outerGhost);
                            XMPI::vSendRecv(MPI,send,fwd.peer,recv,rev.peer,style);
                            asyncLoad(fields,recv,rev.outerGhost);

                        } break;

                        case Connect::AsyncForward: {
                            const AsyncForwardSwaps<COORD> &xch = fragment.asyncForward[iForward++];
                            const Ghosts                   &fwd = *(xch.forward);
                            asyncSave(send,fields,fwd.innerGhost);
                            XMPI::vSend(MPI,send,fwd.peer,style);
                        } break;

                        case Connect::AsyncReverse: {
                            const AsyncReverseSwaps<COORD> &xch = fragment.asyncReverse[iReverse++];
                            const Ghosts                   &rev = *(xch.reverse);
                            asyncMake(recv,rev.outerGhost);
                            XMPI::vRecv(MPI,recv,rev.peer,style);
                            
                        } break;

                        default:
                            assert(Connect::AutoExchange==links.connect || Connect::FreeStanding==links.connect);
                            break;
                    }
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Synchronize);
        };



    }

}

#endif

