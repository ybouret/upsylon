
//! \file

#ifndef Y_PRIORITY_QUEUE_INCLUDED
#define Y_PRIORITY_QUEUE_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {

    namespace core {

        //! Algorithm to format priority queue
        template <typename T>
        struct prio_queue
        {
            Y_DECL_ARGS(T,type); //!< alias


            //! COMPARE( const_type &, const_type &)
            /**
             \param addr user's object address
             \param slot memory where to insert slot[size] is valid
             \param size current objects in slots
             \param compare a data type comparator
             */
            template <typename COMPARE> static inline
            void enqueue(type         *addr,
                         type        **slot,
                         size_t       &size,
                         COMPARE      &compare) throw()
            {
                assert(addr);
                assert(slot);
                //______________________________________________________________
                //
                // place at the end
                //______________________________________________________________
                size_t ipos = size++;
                slot[ipos]  = addr;

                //______________________________________________________________
                //
                // update
                //______________________________________________________________
            PROMOTE:
                const size_t ppos = (ipos-1)>>1;
                if( (ipos>0) && ( compare( *slot[ppos], *slot[ipos] ) < 0 ) )
                {
                    cswap( slot[ppos], slot[ipos] );
                    ipos = ppos;
                    goto PROMOTE;
                }
            }

            //! extract top data and re-balance queue
            template <typename COMPARE> static inline
            type * extract(type   **slot,
                           size_t  &size,
                           COMPARE &compare) throw()
            {
                //______________________________________________________________
                //
                // save top data to return
                //______________________________________________________________
                assert(size>0);
                type *ans = slot[0];

                //______________________________________________________________
                //
                // move last node at top
                //______________________________________________________________
                slot[0]    = slot[--size];
                slot[size] = 0;
                if(size<=0)
                {
                    //__________________________________________________________
                    //
                    // nothing to
                    //__________________________________________________________
                    return ans;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // update
                    //__________________________________________________________
                    size_t ipos = 0;
                RESTORE:
                    const size_t lpos = (ipos<<1) + 1;
                    const size_t rpos = lpos      + 1;

                    //__________________________________________________________
                    //
                    // find would be position
                    //__________________________________________________________
                    size_t mpos = ipos;
                    if( (lpos<size) && (compare( *slot[ipos], *slot[lpos] ) < 0) )
                    {
                        mpos = lpos;
                    }
                    if( (rpos<size) && (compare( *slot[mpos], *slot[rpos] ) < 0) )
                    {
                        mpos =rpos;
                    }

                    //__________________________________________________________
                    //
                    // check positions
                    //__________________________________________________________
                    if(ipos==mpos)
                    {
                        //______________________________________________________
                        //
                        // balanced
                        //______________________________________________________
                        return ans;
                    }
                    else
                    {
                        cswap(slot[ipos],slot[mpos]);
                        ipos = mpos;
                        goto RESTORE;
                    }
                }
            }
        };

        //! priority queue on given memory area
        template <typename T, typename COMPARATOR>
        class priority_queue
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias

            //! setup empty
            inline explicit priority_queue() throw() :
            slot(0), slots(0), count(0) {}

            //! setup
            inline explicit priority_queue(type       **base,
                                           const size_t size) throw() :
            slot(base), slots(size), count(0)
            {
                for(size_t i=0;i<slots;++i) slot[i] = 0;
            }

            //! cleanup
            inline void clear() throw()
            {
                assert(count<=slots);
                while(count>0) slot[--count] = 0;
            }

            //! soft copy queue structure
            inline void store( const priority_queue &other ) throw()
            {
                assert(other.count<=this->slots);
                type       **target    = slot;
                const_type **source    = (const_type **)(other.slot);
                const size_t new_count = other.count;
                while(count>new_count) target[--count] = 0;
                for(size_t i=0;i<new_count;++i)
                {
                    target[i]=(type*)(source[i]);
                }
                count = new_count;
            }

            //! destruct
            inline virtual ~priority_queue() throw()
            {
                clear();
            }

            //! enqueue address of an object
            inline void enqueue( type *addr ) throw()
            {
                assert(count<slots);
                core::prio_queue<T>::template enqueue<COMPARATOR>( (type*)addr,slot,count,compare);
            }

            //! extract address of an object
            inline type *extract( ) throw()
            {
                assert(count>0);
                return prio_queue<T>::template extract<COMPARATOR>( slot, count, compare);
            }

            //! get top item
            inline const_type &peek() const throw()
            {
                assert(count>0);
                return *slot[0];
            }

            //! no-throw swap of data
            inline void swap_with( priority_queue &other ) throw()
            {
                cswap(slot,other.slot);
                _cswap(slots,other.slots);
                cswap(count,other.count);
            }


            type        **slot;    //!< memory
            const size_t  slots;   //!< number or slots
            size_t        count;   //!< number of occupied slots
            COMPARATOR    compare; //!< how to compare things

        private:
            Y_DISABLE_ASSIGN(priority_queue);
        };
    }

    



}


#endif

