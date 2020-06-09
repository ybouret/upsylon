
//! \file
#ifndef Y_MKL_FCN_DERIVATIVES_INCLUDED
#define Y_MKL_FCN_DERIVATIVES_INCLUDED 1

#include "y/mkl/fcn/derivative.hpp"

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
            void grad( array<T> &G, FUNC &F, const array<T> &X, const T h)
            {
                assert(G.size() == X.size());
                array<T>       &Y     = (array<T> &)X;
                grad_call<FUNC> proxy = { &F, 0, &Y };
                size_t         &i     = proxy.iv;
                for(i=X.size();i>0;--i)
                {
                    const T xi = X[i];
                    G[i]        = this->diff(proxy,xi,h);
                    Y[i]       = xi;
                }
            }

            //! derivative of a parametric function dF(x,param)/dx
            template <typename FUNC>
            T parametric_diff( FUNC &F, const T x, array<T> &param, const T h )
            {
                parametric_call<FUNC> proxy = { &F, &param };
                return this->diff(proxy,x,h);
            }

            //! gradient of a parametric function dF(x,param)/dparam
            template <typename FUNC>
            void parametric_grad( array<T> &G, FUNC &F, const T x, const array<T> &param, const T h )
            {
                assert(G.size() == param.size());
                array<T>                  &Param = (array<T> &)param;
                parametric_grad_call<FUNC> proxy = { &F, 0, &Param, x };
                size_t                    &i     = proxy.iv;
                for(i=param.size();i>0;--i)
                {
                    const T p = param[i];
                    G[i]      = this->diff(proxy,p,h);
                    Param[i]  = p;
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

            template <typename FUNC> struct parametric_call
            {
                FUNC     *pF;
                array<T> *pParam;
                inline T operator()(const T x )
                {
                    assert(pF);assert(pParam);
                    return (*pF)(x,*pParam);
                }
            };

            template <typename FUNC> struct parametric_grad_call
            {
                FUNC     *pF;
                size_t    iv;
                array<T> *pParam;
                T         x;
                inline T operator()(const T p)
                {
                    assert(pF);assert(pParam); assert(iv>0); assert(iv<=pParam->size());
                    array<T> &param = *pParam;
                    param[iv] = p;
                    return (*pF)(x,param);
                }
            };




        };
    }
}

#endif

