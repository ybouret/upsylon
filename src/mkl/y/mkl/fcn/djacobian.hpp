
//! \file

#ifndef Y_MATH_FCN_DJACOBIAN_INCLUDED
#define Y_MATH_FCN_DJACOBIAN_INCLUDED 1

#include "y/mkl/fcn/jacobian.hpp"
#include "y/mkl/fcn/derivative.hpp"
#include "y/core/temporary-link.hpp"

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
        class djacobian : public jacobian<T>
        {
        public:
            typedef typename numeric<T>::vector_field field_type;
            typedef typename derivative<T>::pointer   derivator;

            //! cleanup
            virtual ~djacobian() throw() {}

            //! setup
            explicit djacobian( field_type &func, const derivator &drvs) :
            h(1e-4),
            F(func),
            D(drvs),
            i(0),
            j(0)
            {
            }

            T h; //!< scale for all variables

            //! compute the jacobian: J[i][j] = dF_i/d_X[j]
            virtual void operator()(matrix<T> &J,  const accessible<T> &X)
            {
                pV = &J.c_aux1;
                pX = &X;
                const size_t r = J.rows;
                const size_t c = J.cols;
                for(i=r;i>0;--i)
                {
                    array<T> &Ji = J[i];
                    for(j=c;j>0;--j)
                    {
                        Ji[j] = D->diff(*this,X[j],h);
                    }
                }
            }

            //! 1D wrapper
            inline T operator()(const T xtry)
            {
                assert(pV);
                assert(pX);
                addressable<T>      &V    = *pV;
                const accessible<T> &X    = *pX;
                T                   &x    = aliasing::_(X[j]);
                const T              xsav = x;
                try
                {
                    x=xtry;
                    F(V,X);
                    x=xsav;
                    return V[i];
                }
                catch(...)
                {
                    x=xsav;
                    throw;
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(djacobian);

            field_type          &F;
            derivator            D;
            size_t               i;
            size_t               j;
            addressable<T>      *pV;
            const accessible<T> *pX;




        };
    }
}


#endif
