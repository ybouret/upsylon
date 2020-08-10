//! \file

#ifndef Y_MEMORY_JOINT_SECTION_INCLUDED
#define Y_MEMORY_JOINT_SECTION_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/code/ilog2.hpp"

namespace upsylon {

    namespace memory {

        namespace joint {

            //__________________________________________________________________
            //
            //
            //! wrapper for multiple blocks inside a memory area
            //
            //__________________________________________________________________
            class section
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef unsigned_int<sizeof(void*)>::type len_t; //!< ensure aligment

                //! block for internal linked list
                struct block {
                    block   *prev; //!< prev==NULL <=> entry
                    block   *next; //!< next==NULL <=> guard
                    len_t    size; //!< in number of blocks
                    section *from; //!< NULL <=> free
                };

                static const size_t block_size = sizeof(block);               //!< block size
                static const size_t block_exp2 = ilog2<block_size>::value;    //!< for bits shift : block_size = 1 << block_iln2
                static const size_t min_blocks = 4;                           //!< mininum number of blocks for a valid section
                static const size_t min_size   = min_blocks << block_exp2;    //!< minimum size in bytes for a valid section
                static const size_t min_exp2   = ilog2<min_size>::value;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                section(void        *usr_data,
                        const size_t usr_size,
                        const size_t usr_exp2) throw();  //!< create slice, size>=small_size
                ~section()     throw();                  //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                static size_t   bytes_to_hold(const size_t bytes,size_t &shift); //!< memory demand, error if too big
                void            display()                     const; //!< display to debug
                static section *release(void * &, size_t &) throw(); //!< release previously allocated, get owner
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

                

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                block   *entry; //!< entry block
                block   *guard; //!< final block

                section *next; //!< for list
                section *prev; //!< for list

                const len_t size; //!< bytes for blocks, as power of two
                const len_t exp2; //!< size = 1 << exp2       if built with vein
                len_t       prv1; //!< for alignment
                len_t       prv2; //!< for alignment
            private:
                Y_DISABLE_COPY_AND_ASSIGN(section);
                bool  check_block(const block *blk) const;
                bool  check() const;

                typedef void (*finalize)(void *,const size_t);
                void *acquire(size_t &n,finalize) throw();

                void format(const size_t blocks) throw();
            };

        }
    }

}

#endif
