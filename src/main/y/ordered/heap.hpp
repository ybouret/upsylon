//! \file
#ifndef Y_ORDERED_HEAP_INCLUDED
#define Y_ORDERED_HEAP_INCLUDED 1

#include "y/container/container.hpp"
#include "y/comparator.hpp"
#include "y/type/cswap.hpp"
#include <cstring>

namespace upsylon
{

    //! heap built from addresses
    template <typename T,
    typename COMPARATOR = comparator<T>,
    typename ALLOCATOR  = memory::global
    >
    class heap : public container
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias

        //! setup empty
        inline explicit heap() : hmem( ALLOCATOR::instance() ), slots(0), bytes(0), slot(0), count(0), compare()
        {
        }

        //! setup with capacity
        inline explicit heap(const size_t n, const as_capacity_t & ) :
        hmem( ALLOCATOR::instance() ), slots(n), bytes(0), slot( hmem.template acquire_as<slot_t>(slots,bytes) ), count(0), compare()
        {

        }

        //! copy
        inline explicit heap(const heap &other ) :
        hmem( ALLOCATOR::instance() ), slots(other.count), bytes(0), slot( hmem.template acqurire_as<slot_t>(slots,bytes) ), count(other.count), compare()
        {
            assert(slots>=other.count);
            memcpy(slot,other.slot,count*sizeof(slot_t));
        }


        //! desctruct
        inline virtual ~heap() throw()
        {
            __release();
        }

        inline size_t size()     const throw() { return count; } //!< dynamic interface: size
        inline size_t capacity() const throw() { return slots; } //!< dynamic interface: capacity

        inline virtual void free()    throw() { __free();    }   //!< free content
        inline virtual void release() throw() { __release(); }   //!< free and release resources

        //! reserve extra slots
        inline virtual void reserve(const size_t n)
        {
            if(n>0)  __reserve(n);
        }

        //! no-throw swap
        inline void swap_with( heap &other ) throw()
        {
            cswap(slots,other.slots);
            cswap(bytes,other.bytes);
            cswap(slot,other.slot);
            cswap(count,other.count);
        }

        //! insertion of an address
        inline void insert( type *addr )
        {
            //__________________________________________________________________
            //
            // check memory
            //__________________________________________________________________
            assert(addr!=NULL);
            if(count>=slots)
            {
                __reserve( next_increase(count) );
            }
            assert(count<slots);

            //__________________________________________________________________
            //
            // place at the end
            //__________________________________________________________________
            size_t ipos = count;
            size_t ppos = (ipos-1)>>1;
            slot[ipos] = addr;

            //__________________________________________________________________
            //
            // update
            //__________________________________________________________________
            while( (ipos>0) && ( compare( *slot[ppos], *slot[ipos] ) < 0 ) )
            {
                cswap( slot[ppos], slot[ipos] );
                ipos = ppos;
                ppos = (ipos-1)>>1;
            }

            ++count;
        }

        //! peek top data
        const_type & peek() const throw()
        {
            assert(count>0);
            return *slot[0];
        }

        //! extract top data
        type * pop() throw()
        {
            assert(count>0);
            type *ans = slot[0];

            //__________________________________________________________________
            //
            // move last node at top
            //__________________________________________________________________
            slot[0] = slot[--count];

            if(count>0)
            {
                //______________________________________________________________
                //
                // update
                //______________________________________________________________
                size_t ipos = 0;
                size_t lpos = (ipos<<1) + 1;
                size_t rpos = lpos      + 1;

                while(true)
                {
                    //__________________________________________________________
                    //
                    // find would be position
                    //__________________________________________________________
                    size_t mpos = ipos;
                    if(lpos<count && compare( *slot[ipos], *slot[lpos] ) < 0 )
                    {
                        mpos = lpos;
                    }
                    if(rpos<count && compare( *slot[mpos], *slot[rpos] ) < 0 )
                    {
                        mpos =rpos;
                    }

                    //__________________________________________________________
                    //
                    // check positions
                    //__________________________________________________________
                    if(ipos==mpos)
                    {
                        break; // balanced
                    }
                    else
                    {
                        cswap(slot[ipos],slot[mpos]);
                        ipos = mpos;
                        lpos = (ipos<<1) + 1;
                        rpos = lpos      + 1;
                    }
                }

            }

            return ans;
        }



    private:
        Y_DISABLE_ASSIGN(heap);

        typedef mutable_type *slot_t;
        ALLOCATOR &hmem;
        size_t     slots;
        size_t     bytes;
        slot_t    *slot;
        size_t     count;
        COMPARATOR compare;

        inline void __free() throw()
        {
            count = 0;
        }

        inline void __release() throw()
        {
            __free();
            hmem.template release_as<slot_t>(slot,slots,bytes);
        }

        inline void __reserve(const size_t n)
        {
            assert(n>0);
            heap h(slots+n,as_capacity);
            memcpy(h.slot,slot,count*sizeof(slot_t));
            h.count = count;
            count   = 0;
            swap_with(h);
        }
    };

}

#endif

