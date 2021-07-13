//! \file

#ifndef Y_GFX_EDGES_DETECTOR_INCLUDED
#define Y_GFX_EDGES_DETECTOR_INCLUDED 1

#include "y/gfx/edges/keep-max.hpp"
#include "y/gfx/edges/profile.hpp"
#include "y/gfx/ops/gaussian-blur.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {

            typedef gaussian_blur<float> blur;

            class detector
            {
            public:
                explicit detector(const unit_t W, const unit_t H);
                virtual ~detector() throw();

                gradient      grad;
                float         gmax;
                keep_max      kmax;
                profile       prof;
                pixmap<float> temp;

                //! build from pimap<float>
                void build(const pixmap<float>  &field,
                           broker               &apply,
                           const shared_filters &delta,
                           const blur           *cloud);

                //! build with pre-conversion
                template <typename T, typename FUNC> inline
                void build(const pixmap<T>      &source,
                           broker               &apply,
                           FUNC                 &conv,
                           const shared_filters &delta,
                           const blur           *cloud)
                {
                    pixmap<float> &target = (cloud ? grad : temp);
                    //if(cloud) std::cerr << "field=grad" << std::endl; else std::cerr << "field=temp" << std::endl;
                    target.assign(source,apply,conv);
                    build(target,apply,delta,cloud);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(detector);
            };

        }

    }
}

#endif

