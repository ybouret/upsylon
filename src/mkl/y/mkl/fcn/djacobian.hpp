
//! \file

#ifndef Y_MATH_FCN_DJACOBIAN_INCLUDED
#define Y_MATH_FCN_DJACOBIAN_INCLUDED 1

#include "y/mkl/fcn/jacobian.hpp"
#include "y/mkl/fcn/derivative.hpp"
#include "y/core/temporary-link.hpp"
#include "y/core/temporary-value.hpp"

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
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef typename numeric<T>::vector_field field_type; //!< alias
            typedef typename derivative<T>::pointer   derivator;  //!< alias

            //! cleanup
            virtual ~djacobian() throw() {}

            //! setup
            explicit djacobian( field_type &func, const derivator &drvs) :
            h(1e-4),
            F(func),
            D(drvs),
            i(0),
            j(0),
            pV(0),
            pX(0)
            {
            }

            T h; //!< scale for all variables

            //! compute the jacobian: J[i][j] = dF_i/d_X[j]
            virtual void operator()(matrix<T> &J,  const accessible<T> &X)
            {
                core::temporary_link<const accessible<T> > xlink(X,&pX); assert(pX);
                core::temporary_link<addressable<T> >      vlink(J.c_aux1,&pV); assert(pV);
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
                addressable<T>          &V    = *pV;
                const accessible<T>     &X    = *pX;
                core::temporary_value<T> S( aliasing::_(X[j]),xtry);
                F(V,X);
                return V[i];
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
