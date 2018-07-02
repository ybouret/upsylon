//! \file
#ifndef Y_MEMORY_SLICE_INCLUDED
#define Y_MEMORY_SLICE_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{

    namespace memory
    {

        class carver;

        //! format and use user's memory area
        class slice
        {
        public:
            //! internal memory block
            struct block
            {
                block *prev; //!< for internal linked list
                block *next; //!< for internal linked list
                slice *from; //!< for memory retrieval, if NULL, block is available!
                size_t size; //!< available size in bytes
            };
            static const size_t block_size = sizeof(block);   //!< alias
            static const size_t small_size = 2*sizeof(block); //!< critical space

            //! format the buffer with buflen
            /**
             \param buffer memory buffer
             \param buflen >= small_size and is a multiple of block_size
             */
            slice(void        *buffer,
                  const size_t buflen) throw();

            //! bytes to provide to constructor to hold data_size
            static size_t bytes_to_hold( const size_t data_size ) throw();

            //! do nothing but display warning if not empty
            ~slice() throw();

            //! to debug
            void display() const;

            //! try to acquire n bytes
            /**
             if no block is available or big enough, return NULL,
             otherwise, n is aligned to block_size, and memory is zeroed
             */
            void *      acquire(size_t &n) throw();

            //! release the memory
            static slice *release(void * &p, size_t &n) throw();

            //! check status
            bool is_empty() const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(slice);
            block *entry; //!< first item
            block *guard; //!< out of block address
            size_t count; //!< count of available blocks
            void __release(block *curr) throw();
            bool __check() const;
            friend class carver;
            
        public:
            slice *next; //!< for list
            slice *prev; //!< for list
        };

    }


}

#endif

