#include "y/memory/small/stones.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/aliasing.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>

namespace upsylon {

    namespace memory {

        namespace small {

            unsigned stones::width = 8;

            void  stones:: release(stone *s) const throw()
            {
                static global &mgr = global::location();
                assert(NULL!=s);
                mgr.__free(s,bytes);
            }


            size_t stones:: committed() const throw()
            {
                assert(slist.size<=count);
                return count-slist.size;
            }

            stones:: ~stones() throw()
            {
                assert(slist.size<=count);
                if(slist.size<count)
                {
                    const size_t delta = count-slist.size;
                    std::cerr << "[small::stones@" << bytes <<"] missing #stone=" << delta << " -> " << delta*bytes << " bytes" << std::endl;
                }

                while(slist.size)
                {
                    release( slist.pop_back() );
                }
                aliasing::_(count) = 0;
            }

            stones:: stones(const size_t usr_shift) throw() :
            shift(usr_shift),
            bytes(one<<shift),
            slist(),
            count(0)
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
                    void *addr = mgr.__calloc(1,bytes);
                    ++aliasing::_(count);
                    return addr;
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

            std::ostream & operator<<(std::ostream &os, const stones &s)
            {
                os << std::dec;
                os << "{2^" << std::left << std::setw(2) << s.shift << std::right << "=";
                os << std::setw(stones::width) << s.bytes << ": ";
                os <<  "used " << s.committed() << "/" << s.count;
                os << "}";
                return os;
            }

        }
    }
}

