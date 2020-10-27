
#include "y/memory/shack.hpp"
#include "y/type/aliasing.hpp"
#include "y/object.hpp"
#include "y/code/base2.hpp"

#include <cstring>

namespace upsylon {

    namespace memory {

        shack:: ~shack() throw()
        {
            release();
        }

        shack:: shack() throw() :
        block_addr(0),
        block_size(0),
        block_exp2(0),
        count(0),
        width(0),
        clear(0),
        label(0)
        {
        }

        size_t shack:: length() const throw()
        {
            return block_size;
        }


        const void *shack:: ro() const throw()
        {
            return block_addr;
        }

        size_t shack:: tell() const throw()
        {
            return count;
        }

        bool shack:: is_anonymous() const throw()
        {
            if(NULL==label)
            {
                assert(0==count);
                assert(0==width);
                assert(0==clear);
                return true;
            }
            else
            {
                assert(block_addr!=NULL);
                assert(count>0);
                assert(width>0);
                assert(count*width<=block_size);
                assert(clear!=NULL);
                return false;
            }
        }

        bool shack:: is_cplusplus() const throw()
        {
            if(NULL==label)
            {
                assert(0==count);
                assert(0==width);
                assert(0==clear);
                return false;
            }
            else
            {
                assert(block_addr!=NULL);
                assert(count>0);
                assert(width>0);
                assert(count*width<=block_size);
                assert(clear!=NULL);
                return true;
            }
        }

        bool shack:: is_(const std::type_info &tid) const throw()
        {
            return label && (tid==*label);
        }

        void shack:: zero() throw()
        {
            assert(is_anonymous());
            memset(block_addr,0,block_size);
        }

        void shack:: free() throw()
        {
            kill();
            zero();
        }

        void shack:: kill() throw()
        {
            if(is_cplusplus())
            {
                char *base = static_cast<char *>(block_addr);
                base += width * count;
                while(count-- > 0 )
                {
                    clear(base -= width);
                }
                count = 0;
                width = 0;
                clear = 0;
                label = 0;
            }
            assert(is_anonymous());
        }

        void shack:: release() throw()
        {
            if(block_size>0)
            {
                kill();
                assert(is_anonymous());
                object::dyadic_release(block_addr,block_exp2);
                block_addr = 0;
                block_exp2 = 0;
                block_size = 0;
            }
            else
            {
                assert( is_anonymous() );
            }
        }

        void shack:: acquire(size_t n)
        {
            if(n>block_size)
            {
                release();
                block_size = next_power_of_two(n);
                block_exp2 = integer_log2(block_size);
                try
                {
                    block_addr = object::dyadic_acquire(block_exp2);
                    assert( is_zeroed() );
                }
                catch(...)
                {
                    block_size = 0;
                    block_exp2 = 0;
                    throw;
                }

            }
            else
            {
                free();
            }
            assert(block_size>=n);
        }


    }
}
