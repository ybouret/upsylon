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
           // std::cerr << "Looking for " << lhs << std::endl;
            if(num<=0)
            {
                idx = 0;
                return NULL;
            }
            else
            {
                assert(NULL!=arr);
                size_t jlo = 0;
                size_t jup = num-1;
                while(jlo<=jup)
                {
                    //std::cerr << "  --- in [" << jlo << ".." << jup << "]" << std::endl;
                    const size_t jmid = (jlo+jup)>>1;
                    T           *addr  = arr+jmid;
                    const int    icmp  = compare(lhs,*addr);
                    //std::cerr << "  ..compare with " << *addr << "@" << jmid << " -> " << icmp << std::endl;
                    if(icmp<0)
                    {
                        jup = jmid-1;
                        //std::cerr << "   \\_decrease@" << jup << std::endl;
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
                    //std::cerr << "  now in [" << jlo << ".." << jup << "]" << std::endl;
                }
            }
            return NULL;
        }
    }
}

#endif

