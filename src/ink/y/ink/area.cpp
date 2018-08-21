#include "y/ink/area.hpp"
#include "y/parallel.hpp"

namespace upsylon
{
    namespace ink
    {


        areas:: areas( const area &source, const size_t n) : dynamic(), __areas(n,as_capacity)
        {
            //! sanity check
            if(n<=0) return;

            const coord sizes = source.sizes;
            //! full empty
            if(sizes.x<=0||sizes.y<=0)
            {
                const area empty;
                for(size_t r=0;r<n;++r)
                {
                    push_back(empty);
                }
                return;
            }

            assert(sizes.x>0&&sizes.y>0);
            const unit_t cores = n;
            coord split = coord::sqrt_of(cores);
            bool  x_is_small = true;
            bool  y_is_small = false;
            if(sizes.x>sizes.y)
            {
                x_is_small = false;
                y_is_small = true;
                split.swap_xy();
            }

            std::cerr << "should map " << sizes << " to " << split << std::endl;
            if(x_is_small)
            {
                while(split.x>1&&split.x>sizes.x)
                {
                    --split.x;
                    split.y = n/split.x;
                    if(cores!=split.y*split.x) continue;
                }
            }
            else
            {
                while(split.y>1&&split.y>sizes.y)
                {
                    --split.y;
                    split.x = n/split.y;
                    if(cores!=split.y*split.x) continue;
                }
            }
            std::cerr << "found " << split << std::endl;

            for(unit_t j=0;j<split.y;++j)
            {
                std::cerr << "@j=" << j << std::endl;
                const parallel par_y(split.y,j);
                unit_t         y_length = sizes.y;
                unit_t         y_offset = source.lower.y;
                par_y.split(y_length,y_offset);
                for(unit_t i=0;i<split.x;++i)
                {
                    const parallel par_x(split.x,i);
                    unit_t         x_length = sizes.x;
                    unit_t         x_offset = source.lower.x;
                    par_x.split(x_length,x_offset);


                    const coord l(x_offset,y_offset);
                    const coord s(x_length,y_length);
                    const area  a(l,s,area_sizes);
                    std::cerr << "\t" << a.lower << "+" << a.sizes << "=" << a.pixels << std::endl;
                    push_back(a);
                }
            }

        }

    }
}

