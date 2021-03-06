//! \file

#ifndef Y_SPADE_MPI_INCLUDED
#define Y_SPADE_MPI_INCLUDED 1

#include "y/mpi++/xmpi.hpp"
#include "y/spade/workspace.hpp"

namespace upsylon {

    namespace Spade {

        //----------------------------------------------------------------------
        //
        //! handle multiple transfers
        //
        //----------------------------------------------------------------------
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
            template <typename ONE_OR_MORE_FIELD,typename COORD> inline
            void forward(ONE_OR_MORE_FIELD     &fields,
                         const Fragment<COORD> &fragment,
                         IOBlocks              &blocks) const
            {
                assert(blocks.size() == 2*Fragment<COORD>::Levels);
                
                size_t iForward=0;
                size_t iTwoWays=0;
                size_t iReverse=0;
                
                for(unsigned i=0,j=0;i<Fragment<COORD>::Levels;++i)
                {
                    IOBlock                               &send  = blocks[j++];
                    IOBlock                               &recv  = blocks[j++];
                    const typename Dispatch<COORD>::Links &links = fragment.links[i];
                    
                    send.free();
                    switch(links.connect)
                    {
                        case Connect::AsyncTwoWays: {
                            const AsyncTwoWaysSwaps<COORD> &xch = fragment.asyncTwoWays[iTwoWays++];
                            const Ghosts                   &fwd = *(xch.forward);
                            const Ghosts                   &rev = *(xch.reverse);
                            
                            asyncSave(send,fields,fwd.innerGhost);
                            asyncMake(recv,rev.outerGhost);
                            XMPI::vSendRecv(MPI,send,int(fwd.peer),recv,int(rev.peer),style);
                            asyncLoad(fields,recv,rev.outerGhost);

                        } break;

                        case Connect::AsyncForward: {
                            const AsyncForwardSwaps<COORD> &xch = fragment.asyncForward[iForward++];
                            const Ghosts                   &fwd = *(xch.forward);
                            asyncSave(send,fields,fwd.innerGhost);
                            XMPI::vSend(MPI,send,int(fwd.peer),style);
                        } break;

                        case Connect::AsyncReverse: {
                            const AsyncReverseSwaps<COORD> &xch = fragment.asyncReverse[iReverse++];
                            const Ghosts                   &rev = *(xch.reverse);
                            asyncMake(recv,rev.outerGhost);
                            XMPI::vRecv(MPI,recv,int(rev.peer),style);
                            asyncLoad(fields,recv,rev.outerGhost);
                            
                        } break;

                        default:
                            assert(Connect::AutoExchange==links.connect || Connect::FreeStanding==links.connect);
                            break;
                    }
                }
            }
            
            //! reverse waves, after asyncSetup was called
            /**
             send to reverse, recv in forward
             */
            template <typename ONE_OR_MORE_FIELD,typename COORD> inline
            void reverse(ONE_OR_MORE_FIELD     &fields,
                         const Fragment<COORD> &fragment,
                         IOBlocks              &blocks) const
            {
                assert(blocks.size() == 2*Fragment<COORD>::Levels);
                
                size_t iForward=0;
                size_t iTwoWays=0;
                size_t iReverse=0;
                
                for(unsigned i=0,j=0;i<Fragment<COORD>::Levels;++i)
                {
                    IOBlock                               &send  = blocks[j++];
                    IOBlock                               &recv  = blocks[j++];
                    const typename Dispatch<COORD>::Links &links = fragment.links[i];
                    
                    send.free();
                    switch(links.connect)
                    {
                        case Connect::AsyncTwoWays: {
                            const AsyncTwoWaysSwaps<COORD> &xch = fragment.asyncTwoWays[iTwoWays++];
                            const Ghosts                   &fwd = *(xch.forward);
                            const Ghosts                   &rev = *(xch.reverse);
                            
                            asyncSave(send,fields,rev.innerGhost);
                            asyncMake(recv,fwd.outerGhost);
                            XMPI::vSendRecv(MPI,send,int(rev.peer),recv,int(fwd.peer),style);
                            asyncLoad(fields,recv,fwd.outerGhost);
                            
                        } break;
                            
                        case Connect::AsyncForward: {
                            const AsyncForwardSwaps<COORD> &xch = fragment.asyncForward[iForward++];
                            const Ghosts                   &fwd = *(xch.forward);
                            
                            asyncMake(recv,fwd.outerGhost);
                            XMPI::vRecv(MPI,recv,int(fwd.peer),style);
                            asyncLoad(fields,recv,fwd.outerGhost);
                        } break;
                            
                        case Connect::AsyncReverse: {
                            const AsyncReverseSwaps<COORD> &xch = fragment.asyncReverse[iReverse++];
                            const Ghosts                   &rev = *(xch.reverse);
                            asyncSave(send,fields,rev.innerGhost);
                            XMPI::vSend(MPI,send,int(rev.peer),style);
                        } break;
                            
                        default:
                            assert(Connect::AutoExchange==links.connect || Connect::FreeStanding==links.connect);
                            break;
                    }
                }
            }

            
            //! execute full asynhronous swap
            template <typename ONE_OR_MORE_FIELD,typename COORD> inline
            void asyncSwap(ONE_OR_MORE_FIELD     &fields,
                           const Fragment<COORD> &fragment,
                           IOBlocks              &blocks) const
            {
                forward(fields,fragment,blocks);
                reverse(fields,fragment,blocks);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Synchronize);
        };
        
        //----------------------------------------------------------------------
        //
        //! Domain base on workspace
        /**
         use synchronize/transfer to prepare and exchange data
         */
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Domain : public Workspace<COORD>
        {
        public:
            typedef FieldFor<COORD> FieldClass; //!< for templates

            IOBlocks                     blocks; //!< memory for async
            typename Layout<COORD>::Loop loop;   //!< memory for bulk loop

            inline virtual ~Domain() throw() {} //!< cleanup
            
            //! setup
            inline explicit Domain(const mpi           &MPI,
                                   const Layout<COORD> &fullLayout,
                                   const COORD          mapping,
                                   const COORD          boundaries,
                                   const Coord1D        numGhosts) :
            Workspace<COORD>(fullLayout,
                             this->CheckMapping(MPI.size,mapping),
                             MPI.rank,
                             boundaries,
                             numGhosts),
            blocks( Workspace<COORD>::Levels ),
            loop(this->inner.lower,this->inner.upper)
            {
                assert(size_t(MPI.size)==this->size);
                assert(size_t(MPI.rank)==this->rank);
            }

            //! full (blocking) exchange
            /**
             - a proper setupWith(sync) must be called before any exchange
             - a sync.asyncSetup(fields) must be called if the fields are changing
             - this could be used within a thread for compute/comm overlap
             */
            template <typename ONE_OR_MORE_FIELDS> inline
            void exchange(ONE_OR_MORE_FIELDS &fields, const Synchronize &sync)
            {
                Workspace<COORD> &self = *this;
                self.localSwap(fields,sync);
                sync.asyncSwap(fields,*this,blocks);
            }

            //! exchange all async fields
            void exchange(const Synchronize &sync)
            {
                exchange( aliasing::_(this->fields), sync );
            }

            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Domain);
            
        };
        

    }

}

#endif

