#include "y/memory/tight/vein.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/hr-ints.hpp"
#include "y/sort/merge.hpp"
#include "y/os/run-time-log.hpp"
#include <cstring>

namespace upsylon {

    namespace memory {

        namespace tight {

            unsigned vein::width = 8;

            void  vein:: free(ingot *s) const throw()
            {
                static global &mgr = global::location();
                assert(NULL!=s);
                assert(count>0);
                
                mgr.__free(s,block_size);
                --aliasing::_(count);
            }


            size_t vein:: committed() const throw()
            {
                assert(chest.size<=count);
                return count-chest.size;
            }

            size_t vein:: available() const throw()
            {
                return chest.size;
            }

            vein:: ~vein() throw()
            {
                assert(chest.size<=count);
                while(chest.size)
                {
                    free( chest.query() );
                }
                if(count)
                {
                    rtl(run_time_warning) << "[tight::vein@" << block_size <<"] missing #ingot=" << count << " -> " << count*block_size << " bytes" << std::endl;
                }
                aliasing::_(count) = 0;
            }

            vein:: vein(const size_t the_exp2) throw() :
            block_exp2(the_exp2),
            block_size(one<<block_exp2),
            chest(),
            count(0)
            {
                assert(block_exp2>=min_exp2);
                assert(block_exp2<=max_exp2);
            }

            void * vein:: acquire()
            {
                if(chest.size)
                {
                    //----------------------------------------------------------
                    // return an old stone
                    //----------------------------------------------------------
                    return chest.query();
                }
                else
                {
                    //----------------------------------------------------------
                    // create a new ingot
                    return make();
                }
            }

            ingot * vein:: make()
            {
                static global &mgr = global::instance();
                void *addr = mgr.__calloc(1,block_size);
                ++aliasing::_(count);
                return static_cast<ingot *>(addr);
            }

            void vein:: prefetch(size_t n)
            {
                while(n-- > 0 )
                {
                    chest.store( make() );
                }
            }


            
            void vein:: release(void *addr) throw()
            {
                assert(NULL!=addr);
                memset(addr,0,sizeof(ingot));
                (void)chest.store( static_cast<ingot *>(addr) );
            }

            std::ostream & operator<<(std::ostream &os, const vein &s)
            {
                os << std::dec;
                os << "{2^" << std::left << std::setw(2) << s.block_exp2 << std::right << " = ";
                os << std::setw(vein::width) << s.block_size << ": ";
                os <<  "used " << std::setw(3) << s.committed() << "/" << std::setw(3) << s.count;
                const human_readable hr_used = s.committed() * s.block_size;
                const human_readable hr_maxi = s.count * s.block_size;
                os << " ("<< hr_used << "/" << hr_maxi <<")";
                os << "}";
                return os;
            }


            void  vein:: optimize() throw()
            {
                {
                    core::list_of<ingot> ingots;
                    while(chest.size) {
                        ingots.push_back(chest.query());
                    }
                    merging<ingot>::sort_by_increasing_address(ingots);
#if !defined(NDEBUG)
                    {
                        const ingot *node = ingots.head;
                        while(node&&node->next)
                        {
                            assert(node<node->next);
                            node=node->next;
                        }
                    }
#endif

                    while (ingots.size)
                    {
                        chest.store(ingots.pop_front());
                    }
                }

#if !defined(NDEBUG)
                {
                    const ingot *node = chest.head;
                    while(node&&node->next)
                    {
                        assert(node>node->next);
                        node=node->next;
                    }
                }
#endif
            }

            
        }
    }
}

