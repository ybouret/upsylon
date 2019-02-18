
//! \file
#ifndef Y_FIT_ELLIPSE_INCLUDED
#define Y_FIT_ELLIPSE_INCLUDED 1

#include "y/math/fit/fit-conic.hpp"

namespace upsylon
{
    namespace math
    {

        namespace Fit
        {

            //! to fit an algebraic ellipse
            class Ellipse : public Conic
            {
            public:
                explicit Ellipse();         //!< set internal constraint
                virtual ~Ellipse() throw(); //!< destructor

                void factorize( point2d<double> &center );

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ellipse);
            };
        }

    }

}

#endif

