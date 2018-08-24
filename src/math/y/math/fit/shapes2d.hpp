//! \file

#ifndef Y_FIT_SHAPED2D_INCLUDED
#define Y_FIT_SHAPES2D_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/sequence/arrays.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace math
    {
        typedef arrays<double,memory::global> shape2d_arrays;

        class shape2d : public shape2d_arrays
        {
        public:
            virtual ~shape2d() throw();


        protected:
            explicit shape2d(const size_t num_arrays);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(shape2d);
        };

        class fit_circle : public shape2d
        {
        public:
            explicit fit_circle();
            virtual ~fit_circle() throw();

            template <typename ITERATOR>
            bool compute( ITERATOR curr, const size_t n)
            {
                xc=yc=radius=0;
                if(n>0)
                {
                    acquire(n);
                    for(size_t i=n;i>0;--i,++curr)
                    {
                        const double X = double( (*curr).x );
                        const double Y = double( (*curr).y );
                        const double X2 = X*X;
                        const double Y2 = Y*Y;
                        const double Z = X2+Y2;
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
                    return true;
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
            Y_DISABLE_COPY_AND_ASSIGN(fit_circle);
            bool __compute();
            
        public:
            double xc;
            double yc;
            double radius;
        };

    }
}

#endif

