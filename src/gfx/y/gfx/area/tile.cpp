#include "y/gfx/area/tile.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/standard.hpp"
#include "y/parops.hpp"
#include "y/memory/allocator/dyadic.hpp"


namespace upsylon
{
    namespace GFX
    {
        
        HScan:: HScan(const unit_t x, const unit_t y, const unit_t w) throw() :
        begin(x,y),
        width(w),
        x_top(x+w-1)
        {
            assert(w>0);
        }
        
        HScan:: ~HScan() throw()
        {
        }

        Point HScan:: end() const throw()
        {
            return Point(x_top,begin.y);
        }

        
        Tile:: ~Tile() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            mgr.release_as(++hscan,count,bytes);
        }


        unit_t Tile:: items() const throw()
        {
            unit_t sum = 0;
            for(unit_t j=height;j>0;--j)
            {
                assert(hscan[j].width>0);
                sum += hscan[j].width;
            }
            return sum;
        }

        size_t Tile:: size() const throw()
        {
            return height;
        }

        Tile:: Tile(const Area  &area,
                    const size_t sz,
                    const size_t rk) throw() :
        height(0),
        count(0),
        bytes(0),
        hscan(0)
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            assert(sz>0);
            assert(rk<sz);
            if(area.n>0)
            {
                typedef core::standard<unit_t> ustd;

                unit_t length = area.n;
                unit_t offset = 0;

                parops::split_any(length,offset,sz,rk);
                if(length>0)
                {
                    Point lo;
                    {
                        const ustd::div_type l  = ustd::div_call(offset,area.w);
                        const unit_t         dy = l.quot;
                        const unit_t         dx = l.rem;
                        lo = Point(area.x+dx,area.y+dy);
                    }

                    Point up;
                    {
                        const ustd::div_type l  = ustd::div_call(--offset+length,area.w);
                        const unit_t         dy = l.quot;
                        const unit_t         dx = l.rem;
                        up = Point(area.x+dx,area.y+dy);
                    }

                    const unit_t h      = 1+(up.y-lo.y);
                    aliasing::_(height) = size_t(h);
                    count  = height;
                    hscan  = mgr.acquire_as<HScan>(count,bytes);
                    --hscan;
                    
                    if(height<=1)
                    {
                        assert(1==height);
                        new ( &hscan[1] ) HScan(lo.x,lo.y,1+up.x-lo.x);
                    }
                    else
                    {
                        new (&hscan[1]) HScan(lo.x,lo.y,1+area.xm-lo.x);
                        for(unit_t j=2,y=lo.y+1;j<h;++j,++y)
                        {
                            new (&hscan[j])  HScan(area.x,y,area.w);
                        }
                        new (&hscan[height]) HScan(area.x,up.y,1+up.x-area.x);
                    }
                    assert(lower()==lo);
                    assert(upper()==up);
                    assert(items()==length);
                    assert(area.owns(lower()));
                    assert(area.owns(upper()));
                }
                else
                {
                    // data but no length
                    --hscan;
                }
            }
            else
            {
                // no data
                --hscan;
            }
        }

        Point Tile:: lower() const throw()
        {
            if(height)
            {
                return hscan[1].begin;
            }
            else
            {
                return Point();
            }
        }

        Point Tile:: upper() const throw()
        {
            if(height)
            {
                return hscan[height].end();
            }
            else
            {
                return Point();
            }
        }


        std::ostream & operator<<(std::ostream &os, const Tile &s)
        {
            os << '(' << s.lower() << '=' << '>' << s.upper() << ':' << s.items() << ')';
            return os;
        }

        const HScan & Tile:: operator[](const size_t j) const throw()
        {
            return hscan[j];
        }

    }
}
