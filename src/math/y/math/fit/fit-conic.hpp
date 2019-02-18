//! \file
#ifndef Y_FIT_CONIC_INCLUDED
#define Y_FIT_CONIC_INCLUDED 1

#include "y/math/fit/shapes2d.hpp"

namespace upsylon
{
    namespace math
    {

        namespace Fit
        {
            //! fit data points to a conic
            class Conic : public Shape2D
            {
            public:
                static const size_t SIZE = 6; //!< number of parameters

                //! destructor
                virtual ~Conic() throw();

                //! preallocate memory and initialize constraint matrix
                explicit Conic();

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

                    q.ld(0);
                    if(n>0)
                    {
                        acquire(n);
                        Arrays &self = *this;
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
                Y_DISABLE_COPY_AND_ASSIGN(Conic);
                vector<double> sums;
            protected:
                matrix<double> C; //!< 6x6 matrix
            private:
                matrix<double> S; //!< symetric weights matrix
                matrix<double> M; //!< inv(S)*C
                matrix<double> M0;

                bool __compute();
                Arrays          prv;
                array<double>  &wr;  //!< real part      of eigenvalues
                array<double>  &wi;  //!< imaginary part of eigenvalues
            public:
                array<double>  &q; //!< conic parameters
                const matrix<double> &constraint() const throw();
            };

        }
    }

}

#endif



