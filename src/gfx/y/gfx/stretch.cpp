#include "y/gfx/stretch.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/standard.hpp"
#include "y/parops.hpp"
#include "y/memory/allocator/dyadic.hpp"


namespace upsylon
{
    namespace GFX
    {
        
        HTile:: HTile(const unit_t x, const unit_t y, const unit_t w) throw() :
        begin(x,y),
        width(w),
        x_top(x+w-1)
        {
            assert(w>0);
        }
        
        HTile:: ~HTile() throw()
        {
        }
        
        
        Stretch:: ~Stretch() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            
            htile += lower.y;
            mgr.release_as(htile,count,bytes);
        }



        Stretch:: Stretch(const Area  &area,
                          const size_t size,
                          const size_t rank) throw() :
        items(0),
        lower(),
        upper(),
        count(0),
        bytes(0),
        htile(0)
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
                    htile  = mgr.acquire_as<HTile>(count,bytes);
                    htile -= lower.y;
                    
                    if(height<=1)
                    {
                        assert(1==height);
                        new ( &htile[lower.y] ) HTile(lower.x,lower.y,1+upper.x-lower.x);
                    }
                    else
                    {
                        new (&htile[lower.y]) HTile(lower.x,lower.y,1+area.xm-lower.x);
                        for(unit_t j=lower.y+1;j<upper.y;++j)
                        {
                            new (&htile[j]) HTile(area.x,j,area.w);
                        }
                        new (&htile[upper.y]) HTile(upper.x, upper.y, 1+upper.x-area.x);
                    }
                    
                    
#if !defined(NDEBUG)
                    unit_t chk = 0;
                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        assert(htile[j].width>0);
                        chk += htile[j].width;
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

        const HTile & Stretch:: operator[](const unit_t j) const throw()
        {
            assert(j>=lower.y);
            assert(j<=upper.y);
            return htile[j];
        }
        
    }
}
