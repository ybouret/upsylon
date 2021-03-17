

#include "y/gfx/area/tile.hpp"
#include "y/parops.hpp"
#include "y/type/aliasing.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/os/oor.hpp"

namespace upsylon
{
    namespace graphic
    {
        tile:: ~tile() throw()
        {
            static memory::allocator &mgr = memory::dyadic::location();
            ++h_seg;
            out_of_reach::fill(h_seg, 0, bytes);
            mgr.release_as(h_seg, count, bytes);
        }
        
        tile:: tile(const area  &a,
                    const size_t size,
                    const size_t rank)   :
        lines(0),
        cache( new local_memory() ),
        h_seg(0),
        count(0),
        bytes(0),
        shift(0)
        {
            static memory::allocator &mgr = memory::dyadic::instance();
          
            unit_t offset = 0;
            unit_t items  = a.items;
            parops::split_any(items,offset,size,rank);
            
            if(items>0)
            {
                unit_t       sum   = 0;
                const unit_t one   = 1;
                const coord  ini   = a.coord_of(offset);
                const coord  end   = a.coord_of(offset+items-one);
                const unit_t num   = one+end.y-ini.y;
                aliasing::_(lines) = num;

                count = lines;
                h_seg = mgr.acquire_as<segment>(count,bytes)-1;
                switch(lines)
                {
                    case 1:
                        new (h_seg+1) segment(ini.x,ini.y,end.x);
                        sum += h_seg[1].w;
                        break;
                        
                    default:
                        new (h_seg+1)      segment(ini.x,ini.y,a.upper.x);
                        sum += h_seg[1].w;
                        for(unit_t i=2,y=ini.y+1;i<num;++i,++y) {
                            new (h_seg+i) segment(a.lower.x,y,a.upper.x);
                            sum += h_seg[i].w;
                        }
                        new (h_seg+lines)  segment(  a.lower.x,end.y,end.x);
                        sum += h_seg[lines].w;
                        break;
                }
                aliasing::_(shift) = size_t(offset);
                assert(items==sum);
                //std::cerr << "tile: " << ini << " -> " << end << " : lines=" << lines << " | items=" << items << " | shift=" << offset << " | aabb=" << aabb() << std::endl;

            }
            else
            {
                --h_seg;
            }
            
            assert(items==content());
            
          
        }
        
        unit_t tile:: content() const throw()
        {
            unit_t res = 0;
            for(size_t i=lines;i>0;--i)
            {
                res += h_seg[i].w;
            }
            return res;
        }
        
        size_t tile:: size() const throw()
        {
            return lines;
        }
        
        const segment & tile:: operator[](const size_t j) const throw()
        {
            assert(j>0);
            assert(j<=size());
            return h_seg[j];
        }

        coord  tile:: origin()  const throw()
        {
            return h_seg[1].head();
        }
        
        coord  tile:: ending()  const throw()
        {
            return h_seg[lines].tail();
        }

        area tile:: aabb()    const throw()
        {
            unit_t       xmin = h_seg[1].xmin;
            unit_t       xmax = h_seg[1].xmax;
            const unit_t ymin = h_seg[1].y;
            const unit_t ymax = h_seg[lines].y;
            for(size_t i=lines;i>1;--i)
            {
                const segment &s = (*this)[i];
                xmin = min_of(s.xmin,xmin);
                xmax = max_of(s.xmax,xmax);
            }

            return area(xmax-xmin+1,ymax-ymin+1,xmin,ymin);

        }


        std::ostream & operator<<(std::ostream &os, const tile &t)
        {
            os << '{' << t.origin() << " -> " << t.ending() << " : #" << t.content() << '}';
            return os;
        }
    }
    
}
