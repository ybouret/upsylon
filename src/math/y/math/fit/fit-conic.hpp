//! \file
#ifndef Y_FIT_CONIC_INCLUDED
#define Y_FIT_CONIC_INCLUDED 1

#include "y/math/fit/shapes2d.hpp"

namespace upsylon
{
    namespace math
    {

        //! fit data points to a conic
        class fit_conic : public shape2d
        {
        public:
            
            //! destructor
            virtual ~fit_conic() throw();

            //! preallocate memory and initialize constraint matrix
            explicit fit_conic();

            //! construct arrays and perform computation
            template <typename ITERATOR>
            bool compute( ITERATOR curr, const size_t n)
            {
                static const size_t p[8][2] =
                {
                    {0,0}, // unused
                    {2,0},
                    {1,1},
                    {0,2},
                    {1,0},
                    {0,1},
                    {0,0},
                    {0,0} // unused
                };

                a=b=c=d=e=f=0;
                if(n>0)
                {
                    acquire(n);
                    shape2d_arrays &self = *this;
                    for(size_t k=n;k>0;--k,++curr)
                    {
                        const double X = double( (*curr).x );
                        const double Y = double( (*curr).y );

                        size_t l = 0;
                        for(size_t i=6;i>0;--i)
                        {
                            const size_t *ip  = p[i];
                            const double  ixy = ipower(X,ip[0])*ipower(X,ip[1]);
                            for(size_t j=i;j>0;--j)
                            {
                                const size_t *jp  = p[j];
                                const double  jxy = ipower(X,jp[0])*ipower(Y,jp[1]);
                                self[l][k] = ixy*jxy;
                                ++l;
                            }
                        }
                    }
                    return __compute();
                }
                else
                {
                    return true;
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(fit_conic);
            vector<double> sums;
            matrix<double> C;
            double         v; //!< constraint value : A'*C*A=v
            matrix<double> S; //!< symetric weigts matrix
            matrix<double> M; //!< inv(S)*C

            bool __compute();
            
        public:
            double a;
            double b;
            double c;
            double d;
            double e;
            double f;

        };

    }

}

#endif



