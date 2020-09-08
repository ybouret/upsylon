//! \file

#ifndef Y_MATH_FCN_JACOBIAN_INCLUDED
#define Y_MATH_FCN_JACOBIAN_INCLUDED 1

#include "y/mkl/fcn/derivative.hpp"

namespace upsylon
{
    namespace mkl
    {
        //______________________________________________________________________
        //
        //
        //! jacobian computation
        //
        //______________________________________________________________________
        template <typename T>
        class jacobian : public derivative<T>
        {
        public:
            //! cleanup
            virtual ~jacobian() throw() {}
            
            //! setup
            explicit jacobian() : h(1e-4) {}
            
            T h; //!< scale for all variables
            
            //! compute the jacobian: J[i][j] = dF_i/d_X[j]
            template <typename FUNC> inline
            void operator()(matrix<T> &J, FUNC &F, const accessible<T> &X)
            {
                array<T>    &V = J.c_aux1;
                F1D<FUNC>    f = { &F, &V, &X, 0, 0 };
                const size_t r = J.rows;
                const size_t c = J.cols;
                size_t      &i = f.i;
                size_t      &j = f.j;
                for(i=r;i>0;--i)
                {
                    array<T> &Ji = J[i];
                    for(j=c;j>0;--j)
                    {
                        Ji[j] = this->diff(f,X[j],h);
                    }
                }
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(jacobian);
            template <typename FUNC>
            struct F1D
            {
                FUNC                *pF;
                addressable<T>      *pV;
                const accessible<T> *pX;
                size_t               i;
                size_t               j;
                
                inline T operator()(const T xtry)
                {
                    addressable<T>      &V    = *pV;
                    const accessible<T> &X    = *pX;
                    T                   &x    = aliasing::_(X[j]);
                    const T              xsav = x;
                    try
                    {
                        x=xtry;
                        (*pF)(V,X);
                        x=xsav;
                        return V[i];
                    }
                    catch(...)
                    {
                        x=xsav;
                        throw;
                    }
                }
                
            };
        };
    }
}


#endif

