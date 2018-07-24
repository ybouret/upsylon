
//! \file
#ifndef Y_MATH_FCN_DERIVATIVES_INCLUDED
#define Y_MATH_FCN_DERIVATIVES_INCLUDED 1

#include "y/math/fcn/derivative.hpp"

namespace upsylon
{

    namespace math
    {

        //! wrapper for derivatives computation
        template <typename T>
        class derivatives : public derivative<T>
        {
        public:
            //! constructor
            inline explicit derivatives() : derivative<T>() {}

            //! desctructor
            inline virtual ~derivatives() throw() {}

            //! F must be a scalar field. This is not thread safe, uses X as local memory.
            template <typename FUNC>
            void gradient( array<T> &grad, FUNC &F, const array<T> &X, const T h)
            {
                assert(grad.size() == X.size());
                array<T>       &Y = (array<T> &)X;
                grad_call<FUNC> g = { &F, 0, &Y };
                size_t         &i = g.iv;
                for(i=X.size();i>0;--i)
                {
                    const T xi = X[i];
                    grad[i]    = this->compute(g,xi,h);
                    Y[i]       = xi;
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(derivatives);

            template <typename FUNC> struct grad_call
            {
                FUNC     *pF;
                size_t    iv;
                array<T> *pX;
                inline T operator()(const T x)
                {
                    assert(pF);assert(pX); assert(iv>0); assert(iv<=pX->size());
                    array<T> &X    = *pX;
                    X[iv] = x;
                    return (*pF)(X);
                }
            };




        };
    }
}

#endif

