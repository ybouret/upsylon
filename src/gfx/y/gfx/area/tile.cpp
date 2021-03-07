

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
        h_seg(0),
        count(0),
        bytes(0)
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
                std::cerr << "tile: " << ini << " -> " << end << " : lines=" << lines << " | items=" << items << std::endl;
                
                count = lines;
                h_seg = mgr.acquire_as<segment>(count,bytes)-1;
                switch(lines)
                {
                    case 1:
                        new (h_seg+1) segment(ini,end.x-ini.x+one);
                        sum += h_seg[1].length;
                        break;
                        
                    default:
                        new (h_seg+1)      segment(ini,a.upper.x-ini.x+one);
                        sum += h_seg[1].length;
                        for(unit_t i=2,y=ini.y+1;i<num;++i,++y) {
                            new (h_seg+i) segment( coord(a.lower.x,y), a.w );
                            sum += h_seg[i].length;
                        }
                        new (h_seg+lines)  segment( coord(a.lower.x,end.y), end.x-a.lower.x+one );
                        sum += h_seg[lines].length;
                        break;
                }
                
                assert(items==sum);
                
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
                res += h_seg[i].length;
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
        
    }
    
}
