//! \file

#ifndef Y_MEMORY_DBLOCK_INCLUDED
#define Y_MEMORY_DBLOCK_INCLUDED 1

#include "y/core/list.hpp"
#include "y/object.hpp"
#include "y/ptr/ptr.hpp"

namespace upsylon
{

    namespace memory
    {
        class dblocks; //!< forward declaration

        //______________________________________________________________________
        //
        //
        //! low-level data block, cached or not
        //
        //______________________________________________________________________
        class dblock : public object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! smart pointer, using 'zap' to delete
            class pointer : public ptr<dblock>
            {
            public:
                explicit pointer(dblock *blk) throw(); //!< setup blk!=NULL
                virtual ~pointer() throw();            //!< zap(pointee)

            private:
                Y_DISABLE_COPY_AND_ASSIGN(pointer);
            };

            //! core list
            typedef core::list_of<dblock> core_list;

            //! list using 'zap' to delete nodes
            class list : public core_list
            {
            public:
                explicit list() throw();
                virtual ~list() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(list);
            };


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~dblock() throw();                 //!< cleanup
            explicit dblock(const size_t block_exp2);  //!< manual setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            uint8_t *       operator*()       throw();  //!< access
            const uint8_t * operator*() const throw();  //!< access
            static void     zap(dblock *)     throw();  //!< smart delete
            void            ldz()             throw();  //!< zero data
            static size_t   regularize(const size_t block_exp2) throw(); //!< clamp values

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t exp2; //!< size = 2^ exp2
            const size_t size; //!< available bytes
            uint8_t     *addr; //!< bytes
            dblock      *next; //!< for list
            dblock      *prev; //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dblock);
            explicit     dblock(const size_t block_size,
                                const size_t block_exp2,
                                dblocks    * block_org,
                                core_list  * block_src);

            dblocks     *origin; //!< cache
            core_list   *source; //!< slot in cache

            friend class dblocks;
        };

        //______________________________________________________________________
        //
        //
        //! cache of dblocks
        //
        //______________________________________________________________________
        class dblocks
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef dblock::core_list slot_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit dblocks(lockable &); //!< setup with a lock
            virtual ~dblocks() throw();   //!< clean all up

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! access slot in [0..slots-1]
            const slot_type & operator[](const size_t) const throw();

            //! query/create a block
            dblock * query(size_t block_exp2);
            
            //! reserve some memory blocks
            void reserve(size_t n, size_t block_exp2);
            
            //! display (to debug)
            std::ostream        & display( std::ostream &os ) const;
            //! display (to debug)
            friend std::ostream & operator<<(std::ostream &os, const dblocks &db);

            const size_t slots;    //!< base2<size_t>::max_slots
            const size_t mem_size; //!< slots * sizeof(slot_type)
            const size_t mem_exp2; //!< base2<size_t>::log2_of(slots_size)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dblocks);
            slot_type    *slot;    //!< slot[0...slots-1]
            friend class  dblock;

        public:
            lockable     &sync;  //!< shared access
        };

    }
}


#endif
