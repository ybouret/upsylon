

#ifndef Y_MATH_ADJUST_SEQUENTIAL_GRADIENT_INCLUDED
#define Y_MATH_ADJUST_SEQUENTIAL_GRADIENT_INCLUDED 1

#include "y/math/adjust/sequential.hpp"
#include "y/math/fcn/derivatives.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            template <typename T>
            class Gradient : public derivative<T>
            {
            public:
                Y_DECL_ARGS(T,type);
                type h; //!< scaling

                typedef typename Type<type>::Array Array;

                inline explicit Gradient() : derivative<T>(), h(1e-4)
                {
                }

                inline virtual ~Gradient() throw()
                {
                }

                inline void operator()(Array           &dFda,
                                       Sequential<T>   &F,
                                       param_type       x,
                                       const Array      &aorg,
                                       const Variables &vars,
                                       const Flags     &used)
                {
                    assert(used.size()==aorg.size());
                    assert(dFda.size()==aorg.size());
                    // initialize
                    for(size_t j=dFda.size();j>0;--j)
                    {
                        dFda[j] = 0;
                    }

                    // prepare wrapper
                    Wrapper      call = { 0, &F, x, &aorg, &vars  };
                    size_t      &i    = call.i;

                    // loop on variables
                    size_t      nvar  = vars.size();
                    for( Variables::const_iterator v = vars.begin();
                        nvar>0;--nvar,++v)
                    {
                        i = (*v)->index();
                        if(used[i])
                        {
                            dFda[i] = this->diff(call, x, h);
                        }
                    }
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gradient);
                struct Wrapper
                {
                    size_t           i;
                    Sequential<T>   *F_p;
                    type             x;
                    const Array     *aorg_p;
                    const Variables *vars_p;

                    inline type operator()( const_type a )
                    {
                        assert(i>0);
                        assert(F_p);
                        assert(aorg_p);
                        assert(vars_p);
                        
                        Sequential<T>        &F    = *F_p;
                        const addressable<T> &aorg = *aorg_p;
                        mutable_type         &ai   = aliasing::_(aorg)[i];
                        const_type            a0   = ai;
                        try {
                            ai = a;
                            const_type f = F.initialize(x,aorg,*vars_p);
                            ai = a0;
                            return f;
                        }
                        catch(...)
                        {
                            ai = a0;
                            throw;
                        }
                    }

                };
            };

        }

    }

}

#endif

