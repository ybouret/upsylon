
#include "y/memory/small/quarry.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/self-destruct.hpp"
#include "y/exceptions.hpp"
#include "y/memory/allocator/global.hpp"
#include <cstring>
#include <iostream>
#include <cerrno>

namespace upsylon {

    namespace memory {

        namespace small {


            quarry:: ~quarry() throw()
            {
                if(undersized)
                {
                    std::cerr << "[small::quarry] leak #undersized=" << undersized << std::endl;
                }

                for(size_t i=stones::max_shift;i>=stones::min_shift;--i)
                {
                    self_destruct(ore[i]);
                }
                ore=0;
                memset(wksp,0,sizeof(wksp));
                aliasing::_(undersized) = 0;
            }

            quarry:: quarry() throw() :
            ore(0),
            undersized(0),
            wksp()
            {
                memset(wksp,0,sizeof(wksp));
                ore = aliasing::as<stones>(wksp);
                for(size_t i=stones::min_shift;i<=stones::max_shift;++i)
                {
                    new (ore+i) stones(i);
                }
            }

            void  * quarry:: pull(const size_t shift)
            {
                if(shift<stones::min_shift)
                {
                    static global &mgr = global::instance();
                    const  size_t  num = stones::one<<shift;
                    aliasing::_(undersized) += num;
                    return mgr.__calloc(1,num);
                }
                else
                {
                    if(shift>stones::max_shift) throw libc::exception(ERANGE,"quarry::pull(shift>%lu)", (unsigned long) shift );
                    return ore[shift].query();
                }

            }

            void  quarry:: push(void *addr, const size_t shift) throw()
            {
                assert(addr);
                assert(shift<=stones::max_shift);
                if(shift<stones::min_shift)
                {
                    static global &mgr = global::location();
                    const  size_t  num = stones::one<<shift;
                    assert(num<=undersized);
                    aliasing::_(undersized) -= num;
                    return mgr.__free(addr,num);
                }
                else
                {
                    return ore[shift].store(addr);
                }

            }



        }

    }

}
