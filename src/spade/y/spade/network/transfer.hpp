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

        typedef ios::ovstream IOBlock; //!< alias

        //----------------------------------------------------------------------
        //
        //! tranfer of ghosts/swaps
        //
        //----------------------------------------------------------------------
        class Transfer
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            explicit Transfer(const comms::infrastructure where); //!< initialize
            virtual ~Transfer() throw();                           //!< cleanup

            //------------------------------------------------------------------
            //
            // Managing types
            //
            //------------------------------------------------------------------


            //! active a filed based on its objectType
            void     activate( Kernel::Field &F ) const;


            //! query I/O for a given type
            template <typename T> inline
            const ios::conveyor & query()
            {
                return IO.query<T>(infra);
            }

            //! query I/O for a tuple of type
            template <template <typename> class TUPLE, typename T> inline
            const ios::conveyor & query()
            {
                return IO.query<TUPLE,T>(infra);
            }

            //------------------------------------------------------------------
            //
            // local I/O
            //
            //------------------------------------------------------------------

            //! swap local ghosts of one field
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

            //! swap local ghosts for all fields
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

            //! local swap between ghosts
            void localSwap(Kernel::Field &field,
                           const Indices &innerFwd,
                           const Indices &outerFwd,
                           const Indices &innerRev,
                           const Indices &outerRev) const;

            //! local swap between ghosts
            void localSwap(Fields        &fields,
                           const Indices &innerFwd,
                           const Indices &outerFwd,
                           const Indices &innerRev,
                           const Indices &outerRev) const;


            //------------------------------------------------------------------
            //
            // local I/O
            //
            //------------------------------------------------------------------

            //! clear sending block, set style=computed_block_size
            void asyncInitialize(IOBlock &block) throw();

            //! prepare block size according to style
            void asyncAdjustment(IOBlock &recv, const IOBlock &send) const;

            //! save ghost of field into block
            void asyncSave(IOBlock             &block,
                           const Kernel::Field &field,
                           const Ghost         &ghost) const;

            //! save ghost of fields into block
            void asyncSave(IOBlock             &block,
                           Fields              &fields,
                           const Ghost         &ghost) const;

            //! load ghost of fields into block
            void asyncLoad(Kernel::Field &field,
                           ios::istream  &source,
                           const Ghost   &ghost) const;

            //! load ghost of fields into block
            void asyncLoad(Fields        &fields,
                           ios::istream  &source,
                           const Ghost   &ghost) const;


            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------

            const comms::infrastructure infra; //!< global
            const comms::shipping_style style; //!< local, based on types
            ios::conveyors             &IO;    //!< shared database


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Transfer);
            void updateStyleFrom(const ios::conveyor &) const throw();
        };

    }


}


#endif

