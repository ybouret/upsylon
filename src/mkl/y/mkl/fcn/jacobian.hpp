
//! \file
#ifndef Y_MATH_FCN_JACOBIAN_INCLUDED
#define Y_MATH_FCN_JACOBIAN_INCLUDED 1

#include "y/mkl/fcn/derivative.hpp"

namespace upsylon
{
    namespace mkl
    {
        template <typename T>
        class jacobian : public derivative<T>
        {
        public:
            virtual ~jacobian() throw()
            {
            }
            
            explicit jacobian() : h(1e-4)
            {
                
            }
            
            T h;
            
            template <typename FUNC> inline
            void operator()(matrix<T> &J, FUNC &F, const accessible<T> &X)
            {
                array<T>  &V = J.c_aux1;
                F1D<FUNC>  f = { &F, &V, &X, 0, 0 };
                for(f.i=1;f.i<=J.rows;++f.i)
                {
                    for(f.j=1;f.j<=J.cols;++f.j)
                    {
                        J[f.i][f.j] = this->diff(f,X[f.j],h);
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
                
                T operator()(const T xtry)
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

