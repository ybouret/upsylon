
//! \file

#ifndef Y_PRIORITY_QUEUE_INCLUDED
#define Y_PRIORITY_QUEUE_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {

    namespace core {

        //! Algorithm to format priority queue
        template <typename T>
        struct priority_queue
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

            //! extract top data
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
                    size_t lpos = (ipos<<1) + 1;
                    size_t rpos = lpos      + 1;

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

    }

}


#endif

