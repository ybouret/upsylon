//! \file
#ifndef Y_FIT_CIRCLE_INCLUDED
#define Y_FIT_CIRCLE_INCLUDED 1

#include "y/math/fit/in2d/shape.hpp"

namespace upsylon
{
    namespace math
    {

        namespace Fit
        {
            //! fit data points to a circle (Kasa/Coope)
            class Circle : public Shape2D
            {
            public:
                explicit Circle();         //!< initialize
                virtual ~Circle() throw(); //!< destructor

                //! construct arrays then perform computation
                template <typename ITERATOR>
                bool compute( ITERATOR curr, const size_t n)
                {
                    xc=yc=radius=0;
                    if(n>0)
                    {
                        acquire(n);
                        for(size_t i=n;i>0;--i,++curr)
                        {
                            const double X  = double( (*curr).x );
                            const double Y  = double( (*curr).y );
                            const double X2 = X*X;
                            const double Y2 = Y*Y;
                            const double Z  = X2+Y2;
                            x[i]  = X;
                            y[i]  = Y;
                            x2[i] = X2;
                            y2[i] = Y2;
                            xy[i] = X*Y;
                            z[i]  = Z;
                            zx[i] = Z*X;
                            zy[i] = Z*Y;
                        }
                        return __compute();
                    }
                    else
                    {
                        return false; // no points
                    }
                }

            private:
                matrix<double> mu;
                vector<double> rhs;
                array<double> &x;
                array<double> &y;
                array<double> &x2;
                array<double> &y2;
                array<double> &xy;
                array<double> &z;
                array<double> &zx;
                array<double> &zy;
                Y_DISABLE_COPY_AND_ASSIGN(Circle);
                bool __compute();

            public:
                double xc;      //!< fit center_x
                double yc;      //!< fit center_y
                double radius;  //!< fit radius
            };
        }

    }

}


#endif

