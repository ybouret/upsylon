
#include "y/gfx/bitrows.hpp"
#include "y/type/collapse.hpp"
#include "y/gfx/bitrow.hpp"

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
                          const size_t width,
                          const size_t height,
                          const size_t shift) :
        zfw(width),
        impl(height*sizeof(bitrow)),
        done(0)
        {
            assert(entry);
            assert(width>0);
            assert(height>0);
            assert(shift>=width);
            assert(impl.block_size>=height*sizeof(bitrow));
            bitrow *row = static_cast<bitrow *>(impl.block_addr);
            char   *ptr = static_cast<char   *>(entry);
            for(done=0;done<height;++done)
            {
                new (row) bitrow(ptr,width,zfw);
                ++row;
                ptr+=shift;
            }
        }


    }

}

