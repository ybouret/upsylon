
//! \file
#ifndef Y_FIT_ELLIPSE_INCLUDED
#define Y_FIT_ELLIPSE_INCLUDED 1

#include "y/math/fit/in2d/conic.hpp"

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

                //! convert the conic vector q into center+radii+rotation matrix
                void factorize();

                point2d<double>       center; //!< computed center
                point2d<double>       radii;  //!< computed radii, radii.x >= radii.y
                const matrix<double> &rotation() const throw(); //!< computed rotation matrix

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ellipse);
                matrix<double> Q;
                matrix<double> R;
                vector<double> lam;
                void reset() throw();
            };
        }

    }

}

#endif

