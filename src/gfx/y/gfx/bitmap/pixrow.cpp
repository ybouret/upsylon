
#include "y/gfx/bitmap/pixrow.hpp"
#include "y/type/block/zset.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace upsylon
{
    namespace GFX
    {
        PixRow:: PixRow(void           *entry,
                        const unit_t    width,
                        const ZeroFlux &zflux) throw() :
        p(entry),
        w(width),
        z(zflux)
        {
        }

        PixRow:: ~PixRow() throw()
        {
            _bzset(p);
            _bzset(w);
        }

        void * PixRow:: operator()(const unit_t i, const unit_t depth) throw()
        {
            assert(i>=0);
            assert(i<w);
            assert(depth>0);
            return ((uint8_t *)p)+i*depth;
        }

        const void * PixRow:: operator()(const unit_t i, const unit_t depth) const throw()
        {
            assert(i>=0);
            assert(i<w);
            assert(depth>0);
            return ((const uint8_t *)p)+i*depth;
        }


    }
}

#include "y/type/self-destruct.hpp"

namespace upsylon
{
    namespace GFX
    {

        PixRows:: ~PixRows() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            for(unit_t j=h-1;j>=0;--j)
            {
                self_destruct::on( row[j] );
            }
            mgr.release_as(row,count,bytes);
        }

        static inline
        PixRow * acquire_rows(size_t &count,
                              size_t &bytes)
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            return mgr.acquire_as<PixRow>(count,bytes);
        }

        PixRows:: PixRows(const unit_t width,
                          const unit_t height,
                          const void   *entry,
                          const unit_t stride) :
        h(height),
        zfh(height),
        zfw(width),
        count(h),
        bytes(0),
        row( acquire_rows(count,bytes) )
        {
            assert(entry);
            uint8_t *p = (uint8_t *)entry;
            for(unit_t j=0;j<h;++j)
            {
                new (row+j) PixRow(p,width,zfw);
                p += stride;
            }
        }

        PixRow & PixRows:: operator[](const unit_t j) throw()
        {
            assert(j>=0); assert(j<h);
            return row[j];
        }

        const PixRow & PixRows:: operator[](const unit_t j) const throw()
        {
            assert(j>=0); assert(j<h);
            return row[j];
        }


    }
}
