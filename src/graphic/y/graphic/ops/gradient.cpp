#include "y/graphic/ops/gradient.hpp"

namespace upsylon {

    namespace  Graphic {


        Gradient:: ~Gradient() throw()
        {
        }

        Gradient:: Gradient(const size_t W, const size_t H) :
        Pixmap<float>(W,H)
        {
        }

        namespace {

            struct NormTask
            {
                float factor;

                float operator()( const float f ) const throw()
                {
                    return f*factor;
                }

            };
        }

        void Gradient:: normalize(Tiles &tiles) throw()
        {
            // reduce gmax
            const size_t nt   = tiles.count;
            float        gmax = tiles[0].as<float>();

            // scan other tiles
            for(size_t i=1;i<nt;++i)
            {
                const float  tmp  = tiles[i].as<float>();
                if(tmp>gmax) gmax = tmp;
            }

            // check
            if(gmax>0)
            {
                const NormTask task = { 1.0f/gmax };
                Pixmap<float> &self = *this;
                Ops::Run(tiles,self,self,task);
            }
        }


    }

}

