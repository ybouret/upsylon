//! \file
#ifndef Y_YSORT_INCLUDED
#define Y_YSORT_INCLUDED 1

//#include "y/os/platform.hpp"
#include "y/sort/network/all.hpp"

namespace upsylon
{
    //! implement a network sort on N items
#define Y_SORT_IMPL(N)  case N: network_sort<N>::on(&tableau[debut],compare); break

    //! implement a network co-sort on N items
#define Y_SORT_IMPL2(N) case N: nwsrt::co_op##N(&arr[debut],&brr[debut]); break

    //! repeat a macro for network sort
#define Y_SORT_REPEAT(MACRO) \
MACRO( 2); MACRO( 3); MACRO( 4); MACRO( 5); MACRO( 6); MACRO( 7); MACRO( 8); \
MACRO( 9); MACRO(10); MACRO(11); MACRO(12); MACRO(13); MACRO(14); MACRO(15); \
MACRO(16); MACRO(17); MACRO(18); MACRO(19); MACRO(20); MACRO(21); MACRO(22); \
MACRO(23); MACRO(24); MACRO(25); MACRO(26); MACRO(27); MACRO(28); MACRO(30); \
MACRO(31); MACRO(32)

    //! recursive call for quicksort with internal network sort
    template <typename T, typename FUNC>
    inline void _ysort(T           *tableau,
                       const unit_t debut,
                       const unit_t fin,
                       FUNC        &compare)
    {
        if(debut>=fin) return;

        unit_t       gauche = debut;
        unit_t       droite = fin;
        const unit_t objets = fin - (--gauche);

        switch(objets)
        {
                Y_SORT_REPEAT(Y_SORT_IMPL);

            default: {
                const T pivot(tableau[debut]);
                ++droite;

                while(true)
                {
                    do droite--; while( compare(pivot,tableau[droite])<0);
                    do gauche++; while( compare(tableau[gauche],pivot)<0);
                    if(gauche<droite)
                    {
                        memops::swap(tableau[gauche],tableau[droite]);
                    }
                    else
                    {
                        break;
                    }
                }
                _ysort(tableau,debut,droite,compare);
                _ysort(tableau,++droite,fin,compare);
            }
        }
    }

    //! quicksort with network sort for short arrays
    template <typename T, typename FUNC>
    inline void ysort(array<T> &arr, FUNC &compare)
    {
        const unit_t n = unit_t(arr.size());
        _ysort<T>(*arr,0,n-1,compare);
    }
}

#endif

