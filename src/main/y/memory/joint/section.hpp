//! \file

#ifndef Y_MEMORY_JOINT_SECTION_INCLUDED
#define Y_MEMORY_JOINT_SECTION_INCLUDED 1

#include "y/memory/tight/vein.hpp"
#include "y/type/ints.hpp"

namespace upsylon {

    namespace memory {

        namespace joint {

            //__________________________________________________________________
            //
            //
            //! wrapper for multiple blocks inside a power of two memory area
            /**
             made to work with veins
             */
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

                //--------------------------------------------------------------
                //! block for internal linked list
                //--------------------------------------------------------------
                struct block
                {
                    block   *prev; //!< prev==NULL <=> entry
                    block   *next; //!< next==NULL <=> guard
                    len_t    bulk; //!< number of available BLOCKS
                    section *from; //!< NULL <=> free

                    static const size_t size = 4 * sizeof(void*);    //!< size in bytes
                    static const size_t exp2 = ilog2<size>::value;   //!< size=1<<exp2

                    static size_t round(const size_t bytes) throw();               //!< align to block::size
                    inline bool   is_used() const throw() { return NULL!=from;   } //!< alias
                    inline bool   is_free() const throw() { return NULL==from;   } //!< alias
                };

                //--------------------------------------------------------------
                // formatting constants
                //--------------------------------------------------------------
                static const size_t min_blocks = 1<<2;                        //!< mininum number of blocks for a valid section
                static const size_t min_size_  = min_blocks << block::exp2;   //!< minimum size  for a valid section
                static const size_t min_vein_  = tight::vein::min_size;       //!< alias
                static const size_t min_size   = min_size_ > min_vein_ ? min_size_ : min_vein_; //!< ensure a valid vein exists
                static const size_t min_exp2   = ilog2<min_size>::value;      //!< min_size = 1<<min_exp2
                static const size_t max_size   = tight::vein::max_size;       //!< max required size
                static const size_t max_exp2   = tight::vein::max_exp2;       //!< max_size=1<<max_exp2

                //--------------------------------------------------------------
                // I/O constant
                //--------------------------------------------------------------
                static const size_t min_allocated = block::size;              //!< returned valid bytes
                static const size_t max_allocated = max_size - 2*block::size; //!< returned valid bytes

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! format a section
                /**
                 [usr_data]=usr_size=2^usr_exp2, usr_exp2>=min_exp2, usr_size>=min_size
                 */
                section(void        *usr_data,
                        const size_t usr_size,
                        const size_t usr_exp2) throw();
                ~section()     throw();                  //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! compute memory demand
                /**
                 \param bytes query for a section able to hold those bytes
                 \param shift return (1<<shift)
                 by definition, the shift value will always correspond to an
                 existing tight::vein in a tight::quarry
                 */
                static size_t   holding(const size_t bytes,size_t &shift); 
                void            display()                     const; //!< display to debug
                static section *release(void * &, size_t &) throw(); //!< release previously allocated, get owner
                bool            is_empty()            const throw(); //!< check section is free

                //! acquire at least n bytes
                /**
                 upon success, n is updated.
                 upon failure, n is left untouched.
                 */
                void *         acquire(size_t &n)    throw(); //!< try to acquire at least n bytes, zeroed or not


                //! try to change ownership
                /**
                 \param addr current address, updated upon success, left untouched otherwise
                 \param capa current capacity >= size
                 \param size current size at address
                 \return the original owner upon success
                 */
                section *      receive(void * &addr, size_t &capa, const size_t size) throw();

                static section *owner_of(void *p) throw();
                

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                block      *entry;    //!< entry block                         | +1
                block      *guard;    //!< final block                         | +2
                block      *greatest; //!< block with greatest capacity        | +3
                len_t       capacity; //!< greatest number of BYTES            | +4
                section    *next;     //!< for list                            | +5
                section    *prev;     //!< for list                            | +6
                const len_t size;     //!< bytes for all blocks, a power of two| +7
                const len_t exp2;     //!< size = 1 << exp2                    | +8 words


            private:
                Y_DISABLE_COPY_AND_ASSIGN(section);
                bool  check_block(const block *blk) const;
                bool  check() const;

                typedef void (*finalize)(void *,const size_t);
                void  *acquire(size_t &n,finalize) throw();

                void   look_up_greatest() throw(); //!< full search

                //______________________________________________________________
                //
                // greatest API for setup
                //______________________________________________________________
                void   assign_greatest(block *g) throw();               //!< greatest and capacity
                void   assign_greatest(block *lhs, block *rhs) throw(); //!< priority on lhs

                //______________________________________________________________
                //
                // greatest API for post-acquire
                //______________________________________________________________
                void   post_acquire_greatest() throw();  //!< from current
                block *guess_greatest_within(block *lo, block *hi) throw(); //!< local search, called from update_greates

                //______________________________________________________________
                //
                // greatest API for post-release
                //______________________________________________________________
                void post_release_greatest(block *blk) throw();
            };

        }
    }

}

#endif
