//! \file

#ifndef Y_SPADE_TRANSFER_INCLUDED
#define Y_SPADE_TRANSFER_INCLUDED 1

#include "y/spade/layout/fragment.hpp"
#include "y/spade/fields.hpp"
#include "y/ios/conveyors.hpp"
#include "y/ios/ovstream.hpp"

namespace upsylon {
    
    namespace Spade
    {

        
        //----------------------------------------------------------------------
        //
        //! tranfer of ghosts/swaps
        //
        //----------------------------------------------------------------------
        class Transfer
        {
        public:
            typedef ios::ovstream      Block;
            typedef accessible<size_t> Indices;  
            
            explicit Transfer(const comms::topology where);
            virtual ~Transfer() throw();
            
            void     activate( Kernel::Field &F ) const;
            
            
            template <typename COORD>
            void localSwap(Kernel::Field         &field,
                           const Fragment<COORD> &fragment) const
            {
                size_t n = fragment.autoExchange.size();
                while(n-- > 0 )
                {
                    const AutoExchangeSwaps<COORD> &xch = fragment.autoExchange[n];
                    const Ghosts                   &fwd = *(xch.forward);
                    const Ghosts                   &rev = *(xch.reverse);
                    localSwap(field,fwd.innerGhost,fwd.outerGhost,rev.innerGhost,rev.outerGhost);
                }
            }
            
            template <typename COORD>
            void localSwap(Fields                &fields,
                           const Fragment<COORD> &fragment) const
            {
                size_t n = fragment.autoExchange.size();
                while(n-- > 0 )
                {
                    const AutoExchangeSwaps<COORD> &xch = fragment.autoExchange[n];
                    const Ghosts                   &fwd = *(xch.forward);
                    const Ghosts                   &rev = *(xch.reverse);
                    localSwap(fields,fwd.innerGhost,fwd.outerGhost,rev.innerGhost,rev.outerGhost);
                }
            }
            
            
            const comms::topology topology;
            const comms::delivery delivery;
            ios::conveyors       &IO;
            
            
            void asyncInitialize(Block &block) throw();
            
            void localSwap(Kernel::Field &field,
                           const Indices &innerFwd,
                           const Indices &outerFwd,
                           const Indices &innerRev,
                           const Indices &outerRev) const;
            
            void localSwap(Fields        &fields,
                           const Indices &innerFwd,
                           const Indices &outerFwd,
                           const Indices &innerRev,
                           const Indices &outerRev) const;
            
            void asyncSave(Block               &block,
                           const Kernel::Field &field,
                           const Ghost         &ghost) const;
            
            void asyncSave(Block               &block,
                           Fields              &fields,
                           const Ghost         &ghost) const;
            
            void asyncLoad(Kernel::Field &field,
                           ios::istream  &source,
                           const Ghost   &ghost) const;
            
            void asyncLoad(Fields        &fields,
                           ios::istream  &source,
                           const Ghost   &ghost) const;
            

            template <typename T> inline
            const ios::conveyor & query()
            {
                return IO.query<T>(topology);
            }
            
            template <template <typename> class TUPLE, typename T> inline
            const ios::conveyor & query()
            {
                return IO.query<TUPLE,T>(topology);
            }
            
          
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Transfer);
            void updateDelivery(const ios::conveyor &) const throw();
        };
        
        
    }
    
}


#endif

