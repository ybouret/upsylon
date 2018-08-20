#include "y/ink/area.hpp"

namespace upsylon
{
    namespace ink
    {


        areas:: areas( const area &source, const size_t n) : dynamic(), __areas(n,as_capacity)
        {
            if(n>0)
            {
                coord        lower  = source.lower;
                const coord  upper  = source.upper;
                coord        sizes  = source.sizes;
                unit_t       pixels = source.pixels;
                std::cerr << "init sizes=" << sizes << ", #pixels=" << pixels << std::endl;
                for(size_t r=0;r<n;++r)
                {
                    const unit_t pieces      = n-r;
                    const unit_t todo        = pixels/pieces;
                    std::cerr << "-- #pieces=" << pieces << std::endl;
                    if(todo&&pieces<=sizes.x&&pieces<=sizes.y)
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
                            const coord sz = coord(x_length,sizes.y);
                            const area a(lower,sz,area_sizes);
                            push_back(a);
                            lower.x += x_length;
                            sizes.x  = upper.x-lower.x+1;
                            pixels  -= x_pixels;
                        }
                        else
                        {
                            std::cerr << "split_along y" << std::endl;
                            const coord sz = coord(sizes.x,y_length);
                            const area a(lower,sz,area_sizes);
                            push_back(a);
                            lower.y += y_length;
                            sizes.y  = upper.y-lower.y+1;
                            pixels  -= y_pixels;
                        }

                        std::cerr << "+sz" << back().sizes << "@" << back().pixels << std::endl;
                        std::cerr << "|_remaining #pixels=" << pixels << std::endl;
                    }
                    else
                    {
                        std::cerr << "+empty" << std::endl;
                        const area empty(lower,coord(0,0),area_sizes);
                        push_back(empty);
                    }
                }


            }
        }

    }
}

