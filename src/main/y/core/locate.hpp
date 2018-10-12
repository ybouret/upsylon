//! \file
#ifndef Y_CORE_LOCATE_INCLUDED
#define Y_CORE_LOCATE_INCLUDED 1

#include "y/type/bswap.hpp"
#include <iostream>

namespace upsylon
{
    namespace core
    {
        //! locate lhs in an array
        /**
         - using compare(lhs,arr[0..num-1])
         -
         */
        template <typename T,typename U,typename COMPARE>
        T *locate( const U &lhs, T *arr, const size_t num, COMPARE &compare, size_t &idx )
        {
            if(num<=0)
            {
                idx = 0;
                return NULL;
            }
            else
            {
                assert(NULL!=arr);
                size_t jlo = 0;
                {
                    T           *addr  = arr;
                    const int    icmp  = compare(lhs,*addr);
                    if(icmp<0)
                    {
                        idx=0;
                        return NULL;
                    }
                    else if(0==icmp)
                    {
                        idx=0;
                        return addr;
                    }
                    else
                    {
                        ++jlo;
                    }
                }
                size_t jup = num;
                {
                    T        *addr = &arr[ --jup ];
                    const int icmp = compare(lhs,*addr);
                    if(0<icmp)
                    {
                        idx=num;
                        return NULL;
                    }
                    else if(0==icmp)
                    {
                        idx=jup;
                        return addr;
                    }
                    else
                    {
                        --jup;
                    }
                }

                while(jlo<=jup)
                {
                    const size_t jmid = (jlo+jup)>>1;
                    T           *addr  = arr+jmid;
                    const int    icmp  = compare(lhs,*addr);
                    if(icmp<0)
                    {
                        jup = jmid-1;
                    }
                    else
                    {
                        if(icmp>0)
                        {
                            jlo = jmid+1;
                        }
                        else
                        {
                            assert(0==icmp);
                            idx = jmid;
                            return addr;
                        }
                    }
                }
                idx = jlo; assert(compare(lhs,arr[idx])<0);
                return NULL;
            }

        }

        //! combine low-level locate and high level insert_at
        template <typename T,typename VECTOR,typename COMPARE>
        bool insert_sorted( const T &lhs, VECTOR &vec, COMPARE &compare )
        {
            size_t idx = 0;
            if( locate(lhs,*vec,vec.size(),compare,idx) )
            {
                return false;
            }
            else
            {
                vec.insert_at(++idx,lhs);
                return true;
            }
        }
    }
}

#endif

