//! \file

#ifndef Y_MEMORY_DBLOCK_INCLUDED
#define Y_MEMORT_DBLOCK_INCLUDED 1

#include "y/core/list.hpp"
#include "y/object.hpp"
#include "y/ptr/ptr.hpp"

namespace upsylon
{

    namespace memory
    {
        class dblocks;

        //! low-level data block
        class dblock : public object
        {
        public:

            class pointer : public ptr<dblock>
            {
            public:
                explicit pointer(dblock *blk) throw();
                virtual ~pointer() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(pointer);
            };

            typedef core::list_of<dblock> list_type;

            virtual ~dblock() throw();
            explicit dblock(const size_t bs);


            const size_t size; //!< available bytes
            const size_t exp2; //!< block_size = 2^block_exp2

            uint8_t *       operator*()       throw();
            const uint8_t * operator*() const throw();

        public:
            uint8_t     *addr; //!< bytes
            dblock      *next;
            dblock      *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dblock);
            explicit     dblock(const size_t block_size,
                                const size_t block_exp2,
                                dblocks    * block_org,
                                list_type  * block_src);
            
            dblocks     *origin;
            list_type   *source;
            friend class dblocks;
        };

        class dblocks
        {
        public:
            typedef dblock::list_type slot_type;

            explicit dblocks(lockable &);
            virtual ~dblocks() throw();

            const slot_type & operator[](const size_t) const throw();

            const size_t slots;    //!< base2<size_t>::max_slots
            const size_t mem_size; //!< slots * sizeof(slot_type)
            const size_t mem_exp2; //!< base2<size_t>::log2_of(slots_size)

            dblock * query(const size_t bs);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(dblocks);
            slot_type    *slot;    //!< slot[0...slots-1]
            friend class  dblock;

        public:
            lockable     &access;  //!< shared access
        };

    }
}


#endif
