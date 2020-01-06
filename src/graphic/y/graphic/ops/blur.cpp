

#include "y/graphic/ops/blur.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon {

    namespace Graphic  {


        size_t Blur_:: getSizeFor(const float sig) throw()
        {
            const float dmax = clamp( 1.0f, ceilf( sig*3.330f ), 256.0f );
            return size_t( dmax );
        }

        Blur_:: Blur_( const float sig ) :
        sigma( fabsf(sig) ),
        sig2( sigma*sigma ),
        scale(  (sig2>0.0f) ? 1.0f/(sig2+sig2) : 0.0f ),
        delta( getSizeFor(sigma) ),
        upperRange( delta-1 ),
        lowerRange( -upperRange )
        {
        }

        Blur_:: ~Blur_() throw()
        {
            zforce(sigma);
            zforce(sig2);
            zforce(scale);
            zforce(delta);
            zforce(upperRange);
            zforce(lowerRange);
        }



        Blur:: ~Blur() throw()
        {
        }

        Blur:: Blur( const float sig ) :
        Blur_(sig),
        BlurMap( delta, delta )
        {
            BlurMap      &B = *this;
            B[0][0] = 1.0f;
            if(sigma>0.0f)
            {
                // build
                const float   f = -scale;
                const unit_t  w = B->w;
                const unit_t  h = B->h;
                for(unit_t y=0;y<h;++y)
                {
                    BlurMap::RowType &By = B[y];
                    const unit_t      y2 = y*y;
                    for(unit_t x=0;x<w;++x)
                    {
                        const unit_t x2  = x*x;
                        const float  r2  = float(x2+y2);
                        By[x] = expf( f*r2 );
                    }
                }

                // collect weights
                vector<float> weights( (2*w+1)*(2*h+1), as_capacity );
                const unit_t omw = 1-w;
                for(unit_t y=1-h;y<h;++y)
                {
                    const BlurMap::RowType &By = B[ abs_of(y) ];
                    for(unit_t x=omw;x<w;++x)
                    {
                        weights.push_back_( By[ abs_of(x) ] );
                    }
                }

                // normalize
                const float factor = 1.0f / sorted_sum(weights);
                for(unit_t y=0;y<h;++y)
                {
                    BlurMap::RowType &By = B[y];
                    for(unit_t x=0;x<w;++x)
                    {
                        By[x] *= factor;
                    }
                }
            }
        }

        
    }

}

namespace upsylon {

    namespace Graphic  {

        template <>
        void Blur:: apply<float>(Pixmap<float>       &target,
                                 const Pixmap<float> &source,
                                 Tiles               &tiles) const
        {
            apply_<float,float,1>(target,source,tiles);
        }

        template <>
        void Blur:: apply<uint8_t>(Pixmap<uint8_t>       &target,
                                   const Pixmap<uint8_t> &source,
                                   Tiles               &tiles) const
        {
            apply_<uint8_t,uint8_t,1>(target,source,tiles);
        }


        template <>
        void Blur:: apply<rgb>(Pixmap<rgb>       &target,
                               const Pixmap<rgb> &source,
                               Tiles             &tiles) const
        {
            apply_<rgb,uint8_t,3>(target,source,tiles);
        }

        template <>
        void Blur:: apply<rgba>(Pixmap<rgba>       &target,
                                const Pixmap<rgba> &source,
                                Tiles             &tiles) const
        {
            apply_<rgba,uint8_t,3>(target,source,tiles);
        }

        

    }

}


