
#include "y/gfx/bitrows.hpp"
#include "y/type/collapse.hpp"

namespace upsylon
{
    namespace graphic
    {

        bitrows:: ~bitrows() throw()
        {
            bitrow *row = static_cast<bitrow *>(impl.block_addr);
            while(done)
            {
                collapse(row[--done]);
            }
        }

        bitrows:: bitrows(void        *entry,
                          const size_t count,
                          const size_t width,
                          const zflux &zhost,
                          const size_t shift) :
        impl(count*sizeof(bitrow)),
        done(0)
        {
            assert(entry);
            assert(count>0);
            assert(width>0);
            assert(shift>=width);
            bitrow *row = static_cast<bitrow *>(impl.block_addr);
            char   *ptr = static_cast<char   *>(entry);
            for(done=0;done<count;++done)
            {
                new (row) bitrow(ptr,width,zhost);
                ++row;
                ptr+=shift;
            }
        }


    }

}

