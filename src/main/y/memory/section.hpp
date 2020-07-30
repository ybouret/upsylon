//! \file

#ifndef Y_MEMORY_SECTION_INCLUDED
#define Y_MEMORY_SECTION_INCLUDED 1

#include "y/type/ints.hpp"

namespace upsylon {

    namespace memory {

        //! memory bytes
        class section {
        public:
            typedef unsigned_int<sizeof(void*)>::type len_t;

            struct block {
                block   *prev; //!< prev==NULL <=> entry
                block   *next; //!< next==NULL <=> guard
                len_t    size; //!< block length
                section *from; //!< NULL <=> free
            };
            static const size_t block_size = sizeof(block);
            static const size_t min_blocks = 4;
            static const size_t small_size = min_blocks * block_size;
            static const size_t min_size_t = small_size / sizeof(size_t);

            //! create slice, size>=small_size
            section(void *data,const size_t size) throw();
            ~section() throw();
            void display() const;

            block *entry; //!< entry block
            block *guard; //!< final block
            block *large; //!< current largest block

        private:
            Y_DISABLE_COPY_AND_ASSIGN(section);
        };

    }

}

#endif
