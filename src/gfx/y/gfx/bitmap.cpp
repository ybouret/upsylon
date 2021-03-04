#include "y/gfx/bitmap.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{
    namespace graphic
    {
        bitmap:: ~bitmap() throw()
        {
            _bzset(depth);
            _bzset(stride);
        }

        bitmap:: bitmap(const unit_t W, const unit_t H, const unit_t D) :
        entity(),
        area(  checking::gtz(W,checking::width),
               checking::gtz(H,checking::height) ),
        depth( checking::gtz(D,checking::depth) ),
        stride( w*depth ),
        zfh(h),
        zfw(w),
        pixels( new crux::pixels(stride*h) ),
        rows( new bitrows( (void*)(pixels->block_addr),w,h,zfw,stride ) )
        {
        }

        std::ostream & operator<<(std::ostream &os, const bitmap &b)
        {
            const area &a = b;
            os << a << " | depth: " << b.depth << " stride:" << b.stride << " bytes:" << b.pixels->block_size << "+" << b.rows->impl.block_size;
            return os;
        }

    }
}

