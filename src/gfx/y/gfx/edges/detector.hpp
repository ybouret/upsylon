//! \file

#ifndef Y_GFX_EDGES_DETECTOR_INCLUDED
#define Y_GFX_EDGES_DETECTOR_INCLUDED 1

#include "y/gfx/edges/keep-max.hpp"
#include "y/gfx/edges/profile.hpp"
#include "y/gfx/ops/gaussian-blur.hpp"
#include "y/gfx/color/size-to-rgba.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {

            //__________________________________________________________________
            //
            //! gaussian blur for detector: float
            //__________________________________________________________________
            typedef gaussian_blur<float> blur;

            //__________________________________________________________________
            //
            //
            //! embedded edges detector
            //
            //__________________________________________________________________
            class detector : public pixmap<size_t>
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit detector(const unit_t W, const unit_t H); //!< setup
                virtual ~detector() throw();                       //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! prepare from pimap<float>
                size_t prepare(const pixmap<float>  &field,
                               broker               &apply,
                               const shared_filters &delta,
                               const blur           *cloud);

                //! build with pre-conversion
                template <typename T, typename FUNC> inline
                size_t prepare(const pixmap<T>      &source,
                               broker               &apply,
                               FUNC                 &conv,
                               const shared_filters &delta,
                               const blur           *cloud)
                {
                    pixmap<float> &target = (cloud ? grad : temp);
                    target.assign(source,apply,conv);
                    return prepare(target,apply,delta,cloud);
                }

                //! extract blobs (with shared knots)
                size_t extract(blobs        &userBlobs,
                               shared_knots &knotCache);


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                gradient       grad; //!< gradient
                float          gmax; //!< max of gradient (info)
                keep_max       kmax; //!< keep maxima only
                profile        prof; //!< strong/feeble classification
                pixmap<float>  temp; //!< auxiliary for blur or not

            private:
                Y_DISABLE_COPY_AND_ASSIGN(detector);
            };

        }

    }
}

#endif

