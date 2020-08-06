#include "y/memory/small/vein.hpp"
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

            unsigned vein::width = 8;

            void  vein:: release(ingot *s) const throw()
            {
                static global &mgr = global::location();
                assert(NULL!=s);
                assert(count>0);
                //std::cerr << "releasing(" << s << ") #" << count << " bytes=" << bytes << std::endl;
                mgr.__free(s,bytes);
                --aliasing::_(count);
            }


            size_t vein:: committed() const throw()
            {
                assert(cache.size<=count);
                return count-cache.size;
            }

            vein:: ~vein() throw()
            {
                assert(cache.size<=count);
                while(cache.size)
                {
                    release( cache.query() );
                }
                if(count)
                {
                    std::cerr << "[small::vein@" << bytes <<"] missing #ingot=" << count << " -> " << count*bytes << " bytes" << std::endl;
                }
                aliasing::_(count) = 0;
            }

            vein:: vein(const size_t the_shift) throw() :
            shift(the_shift),
            bytes(one<<shift),
            cache(),
            count(0)
            {
                assert(shift>=min_shift);
                assert(shift<=max_shift);
            }

            void * vein:: query()
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


            
            void vein:: store(void *addr) throw()
            {
                assert(NULL!=addr);
                memset(addr,0,sizeof(ingot));
                (void)cache.store( static_cast<ingot *>(addr) );
            }

            std::ostream & operator<<(std::ostream &os, const vein &s)
            {
                os << std::dec;
                os << "{2^" << std::left << std::setw(2) << s.shift << std::right << "=";
                os << std::setw(vein::width) << s.bytes << ": ";
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

