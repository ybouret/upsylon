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

            void  vein:: free(ingot *s) const throw()
            {
                static global &mgr = global::location();
                assert(NULL!=s);
                assert(count>0);
                //std::cerr << "releasing(" << s << ") #" << count << " bytes=" << bytes << std::endl;
                mgr.__free(s,block_size);
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
                    free( cache.query() );
                }
                if(count)
                {
                    std::cerr << "[small::vein@" << block_size <<"] missing #ingot=" << count << " -> " << count*block_size << " bytes" << std::endl;
                }
                aliasing::_(count) = 0;
            }

            vein:: vein(const size_t the_shift) throw() :
            block_exp2(the_shift),
            block_size(one<<block_exp2),
            cache(),
            count(0)
            {
                assert(block_exp2>=min_shift);
                assert(block_exp2<=max_shift);
            }

            void * vein:: acquire()
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
                    // create a new ingot
                    //----------------------------------------------------------
                    void *addr = mgr.__calloc(1,block_size);
                    ++aliasing::_(count);
                    return addr;
                }
            }


            
            void vein:: release(void *addr) throw()
            {
                assert(NULL!=addr);
                memset(addr,0,sizeof(ingot));
                (void)cache.store( static_cast<ingot *>(addr) );
            }

            std::ostream & operator<<(std::ostream &os, const vein &s)
            {
                os << std::dec;
                os << "{2^" << std::left << std::setw(2) << s.block_exp2 << std::right << "=";
                os << std::setw(vein::width) << s.block_size << ": ";
                os <<  "used " << std::setw(3) << s.committed() << "/" << std::setw(3) << s.count;
                const human_readable hr_used = s.committed() * s.block_size;
                const human_readable hr_maxi = s.count * s.block_size;
                os << " ("<< hr_used << "/" << hr_maxi <<")";
                os << "}";
                return os;
            }

        }
    }
}

