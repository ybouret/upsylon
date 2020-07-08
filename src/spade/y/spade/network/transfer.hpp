//! \file

#ifndef Y_SPADE_TRANSFER_INCLUDED
#define Y_SPADE_TRANSFER_INCLUDED 1

#include "y/spade/layout/fragment.hpp"
#include "y/spade/fields.hpp"
#include "y/ios/conveyors.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        typedef ios::ovstream  IOBlock;      //!< alias
        typedef slots<IOBlock> IOBlocksType; //!< alias
        
        //! use as block cache
        class IOBlocks : public IOBlocksType
        {
        public:
            explicit IOBlocks(const size_t levels); //!< create two blocks per level
            virtual ~IOBlocks() throw();            //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(IOBlocks);
        };
        
        
        
        
        //----------------------------------------------------------------------
        //
        //! tranfer of ghosts/swaps, based on conveyors
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
            virtual ~Transfer() throw();                          //!< cleanup
            
            //------------------------------------------------------------------
            //
            // Managing types
            //
            //------------------------------------------------------------------

            //! active a field based on its objectType
            void     setup(Field &F) const;
            
            
            //! query I/O for a given type
            template <typename T> inline
            const ios::conveyor & query() { return IO.query<T>(infra); }
            
            //! query I/O for a tuple of type
            template <template <typename> class TUPLE, typename T> inline
            const ios::conveyor & query() { return IO.query<TUPLE,T>(infra); }
            
            //------------------------------------------------------------------
            //
            // local I/O
            //
            //------------------------------------------------------------------
            
            //! swap local ghosts of one field
            template <typename ONE_OR_MORE_FIELDS, typename COORD>
            void localSwap(ONE_OR_MORE_FIELDS    &fields,
                           const Fragment<COORD> &fragment) const
            {
                //std::cerr << "#localSwaps=" <<fragment.autoExchange.size() << std::endl;
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
            void localSwap(Field         &field,
                           const Indices &innerFwd,
                           const Indices &outerFwd,
                           const Indices &innerRev,
                           const Indices &outerRev) const;
            
            //! local swap between ghosts
            void localSwap(addressable<_Field> &fields,
                           const Indices       &innerFwd,
                           const Indices       &outerFwd,
                           const Indices       &innerRev,
                           const Indices       &outerRev) const;
            
            
            //------------------------------------------------------------------
            //
            // local I/O for one field of some fields
            //
            //------------------------------------------------------------------
            
            //! set style and chunk from field
            void asyncSetup(const Field &field)  throw();
            
            //! set style and chunk from fields
            void asyncSetup(const accessible<_Field> &fields) throw();
            
            //! prepare block from style and chunk
            void asyncMake(IOBlock &block, const Ghost &ghost) const;

            //! save ghost of field into block
            void asyncSave(IOBlock             &block,
                           const Field         &field,
                           const Ghost         &ghost) const;
            
            //! save ghost of fields into block
            void asyncSave(IOBlock                  &block,
                           const accessible<_Field> &fields,
                           const Ghost              &ghost) const;
            
            //! load ghost of fields into block
            void asyncLoad(Field         &field,
                           ios::istream  &source,
                           const Ghost   &ghost) const;
            
            //! load ghost of fields into block
            void asyncLoad(addressable<_Field> &fields,
                           ios::istream        &source,
                           const Ghost         &ghost) const;
            
            //! use internal imstream
            void asyncLoad(Field         &field,
                           const IOBlock &block,
                           const Ghost   &ghost) const;
            
            //! use internal imstream
            void asyncLoad(addressable<_Field> &fields,
                           const IOBlock       &block,
                           const Ghost         &ghost) const;
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const comms::infrastructure infra; //!< global
            const comms::shipping_style style; //!< local, based on types
            const size_t                chunk; //!< bytes per item
            ios::conveyors             &IO;    //!< shared database

            //------------------------------------------------------------------
            //
            // helpers
            //
            //------------------------------------------------------------------

            //! save sub-layout 'bulk' of field into 'block', using 'loop'
            template <typename FIELD>
            inline void asyncBulkSave(IOBlock                          &block,
                                      const FIELD                      &field,
                                      const typename FIELD::LayoutType &bulk,
                                      typename       FIELD::Loop       &loop)
            {
                assert(field.contains(bulk));
                assert(field.io);
                const ios::conveyor &io = *field.io;
                for(loop.reset(bulk.lower,bulk.upper);loop.good();loop.next())
                {
                    io.save(block,&field[*loop]);
                }
            }

            //! load sub-layout 'bulk' of field from 'block', using 'loop'
            template <typename FIELD>
            inline void asyncBulkLoad(FIELD                            &field,
                                      ios::istream                     &source,
                                      const typename FIELD::LayoutType &bulk,
                                      typename       FIELD::Loop       &loop)
            {
                assert(field.contains(bulk));
                assert(field.io);
                const ios::conveyor &io = *field.io;
                for(loop.reset(bulk.lower,bulk.upper);loop.good();loop.next())
                {
                    io.load(&field[*loop],source);
                }
            }

            //! load sub-layout 'bulk' of field from 'block', using 'loop'
            template <typename FIELD>
            inline void asyncBulkLoad(FIELD                            &field,
                                      const IOBlock                    &block,
                                      const typename FIELD::LayoutType &bulk,
                                      typename       FIELD::Loop       &loop)
            {
                ios::imstream source(block);
                asyncBulkLoad(field,source,bulk,loop);
            }



            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Transfer);
        };
        
    }
    
    
}


#endif

