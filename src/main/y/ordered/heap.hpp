//! \file
#ifndef Y_ORDERED_HEAP_INCLUDED
#define Y_ORDERED_HEAP_INCLUDED 1

#include "y/memory/allocator/global.hpp"
#include "y/container/container.hpp"
#include "y/ordered/priority-queue.hpp"
#include "y/comparator.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{

    //! dynamic heap, using internal priority queue
    template <typename T,
    typename COMPARATOR = increasing_comparator<T>,
    typename ALLOCATOR  = memory::global
    >
    class heap : public container
    {
    public:
        //----------------------------------------------------------------------
        //
        // types and declarations
        //
        //----------------------------------------------------------------------
        Y_DECL_ARGS(T,type);                                            //!< aliases
        typedef core::priority_queue<mutable_type,COMPARATOR> pq_type;  //!< alias

        //----------------------------------------------------------------------
        //
        // C++
        //
        //----------------------------------------------------------------------
        //! setup empty
        inline explicit heap(): hmem( ALLOCATOR::instance() ), pq(), bytes(0) {}

        //! setup with capacity
        inline explicit heap(const size_t n, const as_capacity_t &) :
        hmem( ALLOCATOR::instance() ), pq(), bytes(0)
        {
            size_t & capa = aliasing::_(pq.slots);
            capa          = n;
            pq.slot       = hmem.template acquire_as<mutable_type*>(capa,bytes);
        }

        inline virtual ~heap() throw() { __release(); }

        //----------------------------------------------------------------------
        //
        // virtual methods
        //
        //----------------------------------------------------------------------
        inline virtual size_t size()     const throw() { return pq.count; } //!< dynamic interface: size
        inline virtual size_t capacity() const throw() { return pq.slots; } //!< dynamic interface: capacity
        inline virtual void   free()           throw() { pq.clear();      } //!< container interface: free
        inline virtual void   release()        throw() { __release();     } //!< container interface: release
        inline virtual void   reserve(const size_t n)  { __reserve(n);    } //!< container interface: reserve

        //----------------------------------------------------------------------
        //
        // specific methods
        //
        //----------------------------------------------------------------------

        //! extract top data
        inline type *extract() throw()
        {
            return pq.extract();
        }

        //! enqueue data with enough memory
        inline void enqueue_(type *addr) throw()
        {
            pq.enqueue((mutable_type *)addr);
        }

        ///! enqueue data with automatic memory management
        inline void enqueue(type *addr)
        {
            if(pq.count>=pq.slots)
            {
                __reserve( next_increase(pq.count) );
            }
            enqueue_(addr);
        }

        //! check top data
        inline const_type & peek() const throw()
        {
            return pq.peek();
        }
        
    private:
        memory::allocator &hmem;
        pq_type            pq;
        size_t             bytes;

        Y_DISABLE_ASSIGN(heap);

        void __release() throw()
        {
            pq.clear(); assert(0==pq.count);
            hmem.template release_as<mutable_type*>( pq.slot, aliasing::_(pq.slots), bytes);
            assert(0==pq.slot);
            assert(0==pq.slots);
            assert(0==bytes);
        }

        void __reserve(const size_t n)
        {
            if(n>0)
            {
                pq_type &self = pq;
                heap     temp(pq.slots+n,as_capacity);
                pq_type &other = temp.pq;
                other.store(self);
                self.swap_with(other);
                cswap(bytes,temp.bytes);
            }
        }
    };

#if 0
    //! heap built from addresses
    template <typename T,
    typename COMPARATOR = increasing_comparator<T>,
    typename ALLOCATOR  = memory::global
    >
    class heap : public container, public memory::ro_buffer
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias

        //! setup empty
        inline explicit heap() : hmem( ALLOCATOR::instance() ), slots(0), bytes(0), slot(0), count(0), compare() {}

        //! setup with capacity
        inline explicit heap(const size_t n, const as_capacity_t & ) :
        hmem( ALLOCATOR::instance() ), slots(n), bytes(0), slot( hmem.template acquire_as<slot_t>(slots,bytes) ), count(0), compare()
        {
        }

        //! copy constructor
        inline explicit heap(const heap &other ) :
        collection(), container(), memory::ro_buffer(),
        hmem( ALLOCATOR::instance() ), slots(other.count), bytes(0), slot( hmem.template acquire_as<slot_t>(slots,bytes) ), count(other.count), compare()
        {
            assert(slots>=other.count);
            memcpy(slot,other.slot,count*sizeof(slot_t));
        }

        inline virtual            ~heap() throw()           { __release(); }                   //!< destruct
        inline size_t              size()     const throw() { return count; }                  //!< dynamic interface: size
        inline size_t              capacity() const throw() { return slots; }                  //!< dynamic interface: capacity
        inline virtual void        free()    throw()        { __free();    }                   //!< free content
        inline virtual void        release() throw()        { __release(); }                   //!< free and release resources
        inline virtual void        reserve(const size_t n)  { if(n>0) __reserve(n); }          //!< reserve extra slots
        inline virtual size_t      length() const throw()   { return count * sizeof(slot_t); } //!< buffer interface: length
        inline virtual const void *ro() const throw()       { return slot; }                   //!< buffer interface: entry

        //! no-throw swap
        inline void swap_with( heap &other ) throw()
        {
            cswap(slots,other.slots);
            cswap(bytes,other.bytes);
            cswap(slot,other.slot);
            cswap(count,other.count);
        }

        //! insertion of an address with enough memory
        inline void push_( type *addr )
        {
            assert(addr!=NULL);
            assert(count<slots);
            
            //__________________________________________________________________
            //
            // place at the end
            //__________________________________________________________________
            size_t ipos = count;
            slot[ipos]  = addr;
            
            //__________________________________________________________________
            //
            // update
            //__________________________________________________________________
        PROMOTE:
            size_t ppos = (ipos-1)>>1;
            if( (ipos>0) && ( compare( *slot[ppos], *slot[ipos] ) < 0 ) )
            {
                cswap( slot[ppos], slot[ipos] );
                ipos = ppos;
                goto PROMOTE;
            }
            ++count;
        }
        
        //! insertion of an address with memory check
        inline void push( type *addr )
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
            // push item
            //__________________________________________________________________
            push_(addr);
        }

        //! peek top data
        const_type & peek() const throw()
        {
            assert(count>0);
            return *slot[0];
        }

        //! peek second data
        const_type & second() const throw()
        {
            assert(count>=2);
            if(2==count)
            {
                return *slot[1];
            }
            else
            {
                assert(count>=3);
                const_type &lhs = *slot[1];
                const_type &rhs = *slot[2];
                if( compare(lhs,rhs) < 0 )
                {
                    return rhs;
                }
                else
                {
                    return lhs;
                }
            }
        }

        //! extract top data
        type * pop() throw()
        {
            //__________________________________________________________________
            //
            // save top data to return
            //__________________________________________________________________
            assert(count>0);
            type *ans = slot[0];

            //__________________________________________________________________
            //
            // move last node at top
            //__________________________________________________________________
            slot[0] = slot[--count];
            slot[count] = 0;
            if(count<=0)
            {
                //______________________________________________________________
                //
                // nothing to do
                //______________________________________________________________
                return ans;
            }
            else
            {
                //______________________________________________________________
                //
                // update
                //______________________________________________________________
                size_t ipos = 0;
            RESTORE:
                size_t lpos = (ipos<<1) + 1;
                size_t rpos = lpos      + 1;

                //______________________________________________________________
                //
                // find would be position
                //______________________________________________________________
                size_t mpos = ipos;
                if( (lpos<count) && (compare( *slot[ipos], *slot[lpos] ) < 0) )
                {
                    mpos = lpos;
                }
                if( (rpos<count) && (compare( *slot[mpos], *slot[rpos] ) < 0) )
                {
                    mpos =rpos;
                }

                //______________________________________________________________
                //
                // check positions
                //______________________________________________________________
                if(ipos==mpos)
                {
                    return ans; // balanced
                }
                else
                {
                    cswap(slot[ipos],slot[mpos]);
                    ipos = mpos;
                    goto RESTORE;
                }
            }
        }


        inline size_t allocated() const throw()
        {
            return bytes;
        }

    private:
        Y_DISABLE_ASSIGN(heap);

        typedef mutable_type *slot_t;

        ALLOCATOR         &hmem;
        size_t             slots; //!< maximum number of slots
        size_t             bytes;
        slot_t            *slot;
        size_t             count;
        mutable COMPARATOR compare;

        inline void __free() throw()
        {
            memset(slot,0,count*sizeof(slot_t));
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
#endif

}

#endif

