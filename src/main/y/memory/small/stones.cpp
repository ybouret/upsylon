#include "y/memory/small/stones.hpp"
#include "y/memory/allocator/global.hpp"
#include <cstring>

namespace upsylon {

    namespace memory {

        namespace small {

            void  stones:: release(stone *s) const throw()
            {
                static global &mgr = global::location();
                assert(NULL!=s);
                mgr.__free(s,bytes);
            }

            stones:: ~stones() throw()
            {
                while(slist.size) release( slist.pop_back() );
            }

            stones:: stones(const size_t usr_shift) throw() :
            shift(usr_shift),
            bytes(one<<shift),
            slist()
            {
                assert(shift>=min_shift);
                assert(shift<=max_shift);
            }

            void * stones:: query()
            {
                static global &mgr = global::instance();
                if(slist.size)
                {
                    return slist.pop_front();
                }
                else
                {
                    return mgr.__calloc(1,bytes);
                }
            }

            void stones:: store(void *addr) throw()
            {
                assert(NULL!=addr);
                memset(addr,0,sizeof(stone));
                stone *s = slist.push_front( static_cast<stone *>(addr) );
                while(s->next&&s>s->next)
                {
                    slist.towards_tail(s);
                }
            }


        }
    }
}

