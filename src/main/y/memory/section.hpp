//! \file

#ifndef Y_MEMORY_SECTION_INCLUDED
#define Y_MEMORY_SECTION_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/code/ilog2.hpp"

namespace upsylon {

    namespace memory {

        //! memory bytes
        class section {
        public:
            typedef unsigned_int<sizeof(void*)>::type len_t;

            struct block {
                block   *prev; //!< prev==NULL <=> entry
                block   *next; //!< next==NULL <=> guard
                len_t    size; //!< in number of blocks
                section *from; //!< NULL <=> free
            };
            static const size_t block_size = sizeof(block);
            static const size_t block_iln2 = ilog2<block_size>::value;
            static const size_t min_blocks = 3;
            static const size_t small_size = min_blocks * block_size;
            static const size_t min_size_t = small_size / sizeof(size_t);

            //! create slice, size>=small_size
            section(void *data,const size_t size) throw();
            ~section() throw();
            void display() const;

            block *entry; //!< entry block
            block *guard; //!< final block
            
            void           *acquire(size_t &n) throw();
            static section *release(void *addr, size_t &n) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(section);
            bool check_block(const block *blk) const;
            bool check() const;
        };

    }

}

#endif
