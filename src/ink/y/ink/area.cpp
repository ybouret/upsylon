#include "y/ink/area.hpp"

namespace upsylon
{
    namespace ink
    {


        areas:: areas( const area &source, const size_t n) : dynamic(), __areas(n,as_capacity)
        {
            if(n>0)
            {
                coord  lower  = source.lower;
                coord  upper  = source.upper;
                coord  sizes  = source.sizes;
                unit_t pixels = source.pixels;
                
                for(size_t r=0;r<n;++r)
                {
                    const unit_t pieces      = n-r;
                    const unit_t todo        = pixels/pieces;
                    if(todo&&pieces<sizes.x&&pieces<sizes.y)
                    {
                        // cut along x
                        const unit_t x_length    = sizes.x/pieces;
                        const unit_t x_pixels    = x_length * sizes.y;

                        // cut along y
                        const unit_t y_length    = sizes.y/pieces;
                        const unit_t y_pixels    = y_length * sizes.x;
                        std::cerr << sizes << " => pixels=" << pixels << ", todo=" << todo << ", x_pixels=" << x_pixels << ", y_pixels=" << y_pixels << std::endl;
                        const unit_t delta_x     = abs_of(x_pixels-pixels);
                        const unit_t delta_y     = abs_of(y_pixels-pixels);
                        if(delta_x<delta_y)
                        {
                            std::cerr << "split along x" << std::endl;
                            const coord sz(x_length,sizes.y);
                            std::cerr << "\tsz=" << sz << std::endl;
                        }
                        else
                        {
                            std::cerr << "split_along y" << std::endl;
                            const coord sz(sizes.x,y_length);
                            std::cerr << "\tsz=" << sz << std::endl;
                        }
                    }
                    else
                    {
                        //empty area
                    }
                    exit(1);
                }


            }
        }

    }
}

