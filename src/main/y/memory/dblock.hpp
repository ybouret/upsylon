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


            typedef core::list_of<dblock> core_list;

            class list : public core_list
            {
            public:
                explicit list() throw();
                virtual ~list() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(list);
            };

            virtual ~dblock() throw();
            explicit dblock(const size_t bs);



            uint8_t *       operator*()       throw();
            const uint8_t * operator*() const throw();
            static   void zap(dblock *pointee) throw();

            const size_t size; //!< available bytes
            const size_t exp2; //!< block_size = 2^block_exp2
        public:
            uint8_t     *addr; //!< bytes
            dblock      *next;
            dblock      *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dblock);
            explicit     dblock(const size_t block_size,
                                const size_t block_exp2,
                                dblocks    * block_org,
                                core_list  * block_src);
            
            dblocks     *origin;
            core_list   *source;
            friend class dblocks;
        };

        
        class dblocks
        {
        public:
            typedef dblock::core_list slot_type;

            explicit dblocks(lockable &);
            virtual ~dblocks() throw();

            const slot_type & operator[](const size_t) const throw();

            dblock * query(const size_t bs);
            
            std::ostream        & display( std::ostream &os ) const;
            friend std::ostream & operator<<(std::ostream &os, const dblocks &db);

            const size_t slots;    //!< base2<size_t>::max_slots
            const size_t mem_size; //!< slots * sizeof(slot_type)
            const size_t mem_exp2; //!< base2<size_t>::log2_of(slots_size)

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
