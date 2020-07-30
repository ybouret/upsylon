//! \file

#ifndef Y_MEMORY_SECTION_INCLUDED
#define Y_MEMORY_SECTION_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/code/ilog2.hpp"

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! wrapper for multiple blocks inside a memory area
        //
        //______________________________________________________________________
        class section
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________

            typedef unsigned_int<sizeof(void*)>::type len_t; //!< ensure aligment

            //! block for internal linked list
            struct block {
                block   *prev; //!< prev==NULL <=> entry
                block   *next; //!< next==NULL <=> guard
                len_t    size; //!< in number of blocks
                section *from; //!< NULL <=> free
            };

            static const size_t block_size = sizeof(block);               //!< block size
            static const size_t block_iln2 = ilog2<block_size>::value;    //!< for bits shift
            static const size_t min_blocks = 3;                           //!< mininum number of blocks
            static const size_t small_size = min_blocks * block_size;     //!< minimum size to build
            static const size_t min_size_t = small_size / sizeof(size_t); //!< minimum number of size_t

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! create slice, size>=small_size
            section(void *data,const size_t size) throw();

            ~section()     throw(); //! cleanup

            void            display()                     const; //!< display to debug
            static section *release(void * &, size_t &) throw(); //!< release perviously allocated, get owner
            bool            is_free()             const throw(); //!< check section is free

            //! acquire at least n bytes
            /**
             upon success, n is updated.
             upon failure, n is left untouched.
             */
            void *         acquire(size_t &n)    throw(); //!< try to acquire at least n bytes, zeroed or not

            //! try to change ownership
            /**
             \param addr current address, updated upon success, left untouched otherwise
             \param maxi current capacity >= size
             \param size current size at address
             \return the original owner upon success
             */
            section *      receive(void * &addr, size_t &maxi, const size_t size) throw();


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            block *entry; //!< entry block
            block *guard; //!< final block


        private:
            Y_DISABLE_COPY_AND_ASSIGN(section);
            bool  check_block(const block *blk) const;
            bool  check() const;
            void *try_acquire(size_t &,const bool) throw();
            
        };

    }

}

#endif
