#include "y/memory/small/stones.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/hr-ints.hpp"
#include "y/sort/merge.hpp"
#include <iostream>
#include <cstring>
#include <iomanip>

namespace upsylon {

    namespace memory {

        namespace small {

            unsigned stones::width = 8;

            void  stones:: release(stone *s) const throw()
            {
                static global &mgr = global::location();
                assert(NULL!=s);
                assert(count>0);
                mgr.__free(s,bytes);
                --aliasing::_(count);
            }


            size_t stones:: committed() const throw()
            {
                assert(cache.size<=count);
                return count-cache.size;
            }

            stones:: ~stones() throw()
            {
                assert(cache.size<=count);
                while(cache.size)
                {
                    release( cache.query() );
                }
                if(count)
                {
                    std::cerr << "[small::stones@" << bytes <<"] missing #stone=" << count << " -> " << count*bytes << " bytes" << std::endl;
                }
                aliasing::_(count) = 0;
            }

            stones:: stones(const size_t usr_shift) throw() :
            shift(usr_shift),
            bytes(one<<shift),
            cache(),
            count(0)
            {
                assert(shift>=min_shift);
                assert(shift<=max_shift);
            }

            void * stones:: query()
            {
                static global &mgr = global::instance();
                if(cache.size)
                {
                    //----------------------------------------------------------
                    // return an old stone
                    //----------------------------------------------------------
                    return cache.query();
                }
                else
                {
                    //----------------------------------------------------------
                    // create a new stone
                    //----------------------------------------------------------
                    void *addr = mgr.__calloc(1,bytes);
                    ++aliasing::_(count);
                    return addr;
                }
            }


            
            void stones:: store(void *addr) throw()
            {
                assert(NULL!=addr);
                memset(addr,0,sizeof(stone));
                (void)cache.store( static_cast<stone *>(addr) );
            }

            std::ostream & operator<<(std::ostream &os, const stones &s)
            {
                os << std::dec;
                os << "{2^" << std::left << std::setw(2) << s.shift << std::right << "=";
                os << std::setw(stones::width) << s.bytes << ": ";
                os <<  "used " << std::setw(3) << s.committed() << "/" << std::setw(3) << s.count;
                const human_readable hr_used = s.committed() * s.bytes;
                const human_readable hr_maxi = s.count * s.bytes;
                os << " ("<< hr_used << "/" << hr_maxi <<")";
                os << "}";
                return os;
            }

        }
    }
}

