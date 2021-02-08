#include "y/gfx/stretch.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/standard.hpp"
#include "y/parops.hpp"



namespace upsylon
{
    namespace GFX
    {
        Stretch:: ~Stretch() throw()
        {
        }

        Stretch:: Stretch(const Area  &area,
                          const size_t size,
                          const size_t rank) throw() :
        items(0),
        lower(),
        upper()
        {
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


                }

            }
        }


        std::ostream & operator<<(std::ostream &os, const Stretch &s)
        {
            os << '(' << s.lower << '=' << '>' << s.upper << ':' << s.items << ')';
            return os;
        }


    }
}
