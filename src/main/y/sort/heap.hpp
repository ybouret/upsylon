//! \file
#ifndef Y_SORT_HEAP_INCLUDED
#define Y_SORT_HEAP_INCLUDED 1

#include "y/sequence/addressable.hpp"
#include "y/memory/io.hpp"
#include "y/code/round.hpp"
#include "y/type/block/move.hpp"
#include <cstring>

namespace upsylon
{
    

    //! heap sort
    template <typename T,typename FUNC>
    inline void hsort( addressable<T> &ra, FUNC &compare ) throw()
    {
        const size_t n = ra.size();
        if (n < 2) return;

        //----------------------------------------------------------------------
        // local memory
        //----------------------------------------------------------------------
        char wksp[ Y_MEMALIGN(sizeof(T)) ]; assert(sizeof(wksp)>=sizeof(T));
        T   &rra = * memory::io::__force<T>( &wksp[0] );

        //----------------------------------------------------------------------
        // algorithm
        //----------------------------------------------------------------------
        size_t l =(n >> 1)+1;
        size_t ir=n;
        for (;;)
        {
            if (l>1)
            {
                bmove(rra,ra[--l]);
            }
            else
            {
                bmove( rra,    ra[ir] );
                bmove( ra[ir], ra[1]  );
                if (--ir == 1)
                {
                    bmove(ra[1],rra);
                    break;
                }
            }
            size_t i=l;
            size_t j=l+l;
            while (j <= ir)
            {
                if (j < ir && compare(ra[j],ra[j+1]) < 0 )
                {
                    j++;
                }
                if ( compare(rra,ra[j]) < 0)
                {
                    bmove( ra[i], ra[j] );
                    i=j;
                    (j<<=1);
                }
                else
                {
                    j=ir+1;
                }
            }
            bmove( ra[i], rra );
        }
    }


    //! heap co sort
    template <typename T, typename U, typename FUNC>
    inline void hsort( addressable<T> &ra, addressable<U> &rb, FUNC &compare ) throw()
    {
        const size_t n = ra.size(); assert( ra.size() == rb.size() );
        if (n<2) return;

        //----------------------------------------------------------------------
        //-- local memory
        //----------------------------------------------------------------------
        char wksp[ Y_MEMALIGN(sizeof(T)) ]; assert(sizeof(wksp)>=sizeof(T));
        T   &rra = * memory::io::__force<T>(&wksp[0]);

        char wksp2[ Y_MEMALIGN(sizeof(U)) ]; assert(sizeof(wksp2)>=sizeof(U));
        U   &rrb  = * memory::io::__force<U>(&wksp2[0]);

        //----------------------------------------------------------------------
        //-- algorithm
        //----------------------------------------------------------------------
        size_t l =(n >> 1)+1;
        size_t ir=n;
        for (;;)
        {
            if (l>1)
            {
                --l;
                bmove(rra,ra[l]);
                bmove(rrb,rb[l]);
            }
            else
            {
                bmove( rra,    ra[ir] );
                bmove( ra[ir], ra[1]  );

                bmove( rrb,    rb[ir] );
                bmove( rb[ir], rb[1]  );

                if (--ir == 1)
                {
                    bmove(ra[1],rra);
                    bmove(rb[1],rrb);
                    break;
                }
            }
            size_t i=l;
            size_t j=l+l;
            while (j <= ir)
            {
                if(j < ir && compare(ra[j],ra[j+1]) < 0 )
                    j++;
                if( compare(rra,ra[j]) < 0)
                {
                    bmove( ra[i], ra[j] );
                    bmove( rb[i], rb[j] );
                    i=j;
                    (j <<= 1);
                }
                else
                {
                    j=ir+1;
                }
            }
            bmove( ra[i], rra );
            bmove( rb[i], rrb );
        }
    }
}
#endif

