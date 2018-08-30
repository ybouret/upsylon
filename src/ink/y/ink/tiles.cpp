#include "y/ink/tiles.hpp"
#include "y/parallel.hpp"

namespace upsylon
{
    namespace Ink
    {


        Tiles:: Tiles( const Area &source, const size_t n) : dynamic(), __Tiles(n,as_capacity)
        {
            //! sanity check
            if(n<=0) return;

            const size_t w = source.w;
            const size_t h = source.h;
            //! full empty
            if(w<=0||h<=0)
            {
                const Area empty;
                for(size_t r=0;r<n;++r)
                {
                    push_back(empty);
                }
                return;
            }

            assert(w>0&&h>0);
            const unit_t cores = n;
            coord split = coord::sqrt_of(cores);
            bool  w_is_small = true;
            if(w>h)
            {
                w_is_small = false;
                split.swap_xy();
            }

            std::cerr << "should map " << w << 'x' << h  << " to " << split << std::endl;
            if(w_is_small)
            {
                while(split.x>1&&size_t(split.x)>w)
                {
                    --split.x;
                    split.y = n/split.x;
                    if(cores!=split.y*split.x) continue;
                }
            }
            else
            {
                while(split.y>1&&size_t(split.y)>h)
                {
                    --split.y;
                    split.x = n/split.y;
                    if(cores!=split.y*split.x) continue;
                }
            }
            std::cerr << "found " << split << std::endl;

            for(unit_t j=0;j<split.y;++j)
            {
                //std::cerr << "@j=" << j << std::endl;
                const parallel par_y(split.y,j);
                unit_t         y_length = h;
                unit_t         y_offset = source.lower.y;
                par_y.split(y_length,y_offset);
                for(unit_t i=0;i<split.x;++i)
                {
                    const parallel par_x(split.x,i);
                    unit_t         x_length = w;
                    unit_t         x_offset = source.lower.x;
                    par_x.split(x_length,x_offset);


                    const coord l(x_offset,y_offset);
                    //const coord s(x_length,y_length);
                    const Area  a(l,x_length,y_length);
                    const Tile  t(a);
                    //std::cerr << "\t" << a.lower << "+" << a.sizes << "=" << a.pixels << std::endl;
                    push_back(t);
                }
            }

        }

    }
}

