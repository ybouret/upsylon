#include "y/gfx/area/stretch.hpp"
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
        
        
        Stretch:: ~Stretch() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            
            hscan += lower.y;
            mgr.release_as(hscan,count,bytes);
        }



        Stretch:: Stretch(const Area  &area,
                          const size_t size,
                          const size_t rank) throw() :
        items(0),
        lower(),
        upper(),
        count(0),
        bytes(0),
        hscan(0)
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            assert(size>0);
            assert(rank<size);
            if(area.n>0)
            {
                typedef core::standard<unit_t> ustd;

                unit_t length = area.n;
                unit_t offset = 0;

                parops::split_any(length,offset,size,rank);
                if(length>0)
                {
                    aliasing::_(items) = length;
                    {
                        const ustd::div_type l  = ustd::div_call(offset,area.w);
                        const unit_t         dy = l.quot;
                        const unit_t         dx = l.rem;
                        aliasing::_(lower)      = Point(area.x+dx,area.y+dy);
                    }

                    {
                        const ustd::div_type l  = ustd::div_call(--offset+length,area.w);
                        const unit_t         dy = l.quot;
                        const unit_t         dx = l.rem;
                        aliasing::_(upper)      = Point(area.x+dx,area.y+dy);
                    }
                    const unit_t height = 1+(upper.y-lower.y);
                    count  = height;
                    hscan  = mgr.acquire_as<HScan>(count,bytes);
                    hscan -= lower.y;
                    
                    if(height<=1)
                    {
                        assert(1==height);
                        new ( &hscan[lower.y] ) HScan(lower.x,lower.y,1+upper.x-lower.x);
                    }
                    else
                    {
                        new (&hscan[lower.y]) HScan(lower.x,lower.y,1+area.xm-lower.x);
                        for(unit_t j=lower.y+1;j<upper.y;++j)
                        {
                            new (&hscan[j]) HScan(area.x,j,area.w);
                        }
                        new (&hscan[upper.y]) HScan(upper.x, upper.y, 1+upper.x-area.x);
                    }
                    
                    
#if !defined(NDEBUG)
                    unit_t chk = 0;
                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        assert(hscan[j].width>0);
                        chk += hscan[j].width;
                    }
                    assert(chk==items);
#endif
                }

            }
        }


        std::ostream & operator<<(std::ostream &os, const Stretch &s)
        {
            os << '(' << s.lower << '=' << '>' << s.upper << ':' << s.items << ')';
            return os;
        }

        const HScan & Stretch:: operator[](const unit_t j) const throw()
        {
            assert(j>=lower.y);
            assert(j<=upper.y);
            return hscan[j];
        }
        
    }
}
