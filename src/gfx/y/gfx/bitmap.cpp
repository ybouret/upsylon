#include "y/gfx/bitmap.hpp"
#include "y/type/block/zset.hpp"
#include "y/gfx/bitrow.hpp"

#include <cstring>

namespace upsylon
{
    namespace graphic
    {
        bitmap:: ~bitmap() throw()
        {
            _bzset(depth);
            _bzset(scanline);
            _bzset(stride);
        }

        bitmap:: bitmap(const unit_t W, const unit_t H, const unit_t D) :
        entity(),
        area(  checking::gtz(W,checking::width),
               checking::gtz(H,checking::height) ),
        depth( checking::gtz(D,checking::depth) ),
        scanline(w*depth),
        stride( scanline),
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
        scanline(other.scanline),
        stride(other.stride),
        zfh(other.zfh),
        zfw(other.zfw),
        pixels(other.pixels),
        a_rows(other.a_rows)
        {

        }

        void bitmap:: ldz() throw()
        {
            char *p = static_cast<char *>(get_line(0));
            for(unit_t j=h;j>0;--j,p+=stride)
            {
                memset(p,0,scanline);
            }
        }


        std::ostream & operator<<(std::ostream &os, const bitmap &b)
        {
            const area &a = b;
            os << a << " | depth: " << b.depth << " stride:" << b.stride << " bytes:" << b.pixels->block_size << "+" << b.a_rows->impl.block_size;
            return os;
        }


        void *bitmap:: row_address(const unit_t y) throw()
        {
            assert(y>=0); assert(y<h);
            return static_cast<bitrow *>(a_rows->impl.block_addr)+y;
        }

        const void *bitmap:: row_address(const unit_t y) const throw()
        {
            assert(y>=0); assert(y<h);
            return static_cast<bitrow *>(a_rows->impl.block_addr)+y;
        }

        void * bitmap:: get_line(const unit_t y) throw()
        {
            return (static_cast<bitrow *>(a_rows->impl.block_addr)+y)->p;
        }

        const void * bitmap:: get_line(const unit_t y) const throw()
        {
            return (static_cast<const bitrow *>(a_rows->impl.block_addr)+y)->p;
        }

        void *bitmap:: at(const unit_t x, const unit_t y) throw()
        {
            assert(owns(x,y));
            return static_cast<bitrow *>( row_address(y) )->at(x,depth);
        }

        void *bitmap:: at(const coord p) throw()
        {
            assert(owns(p));
            return at(p.x,p.y);
        }

        const void *bitmap:: at(const unit_t x, const unit_t y) const throw()
        {
            assert(owns(x,y));
            return static_cast<const bitrow *>( row_address(y) )->at(x,depth);
        }

        const void *bitmap:: at(const coord p) const throw()
        {
            assert(owns(p));
            return at(p.x,p.y);
        }

    }
}

