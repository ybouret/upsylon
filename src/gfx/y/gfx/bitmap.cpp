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
        a_rows( new bitrows( (void*)(pixels->block_addr),w,h,zfw,stride ) )
        {
        }

        bitmap:: bitmap(const bitmap &other) throw() :
        entity(),
        area(other),
        depth(other.depth),
        stride(other.stride),
        zfh(other.zfh),
        zfw(other.zfw),
        pixels(other.pixels),
        a_rows(other.a_rows)
        {

        }

        std::ostream & operator<<(std::ostream &os, const bitmap &b)
        {
            const area &a = b;
            os << a << " | depth: " << b.depth << " stride:" << b.stride << " bytes:" << b.pixels->block_size << "+" << b.a_rows->impl.block_size;
            return os;
        }

    }
}

