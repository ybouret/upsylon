

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

                explicit Gradient() : derivative<T>(), h(1e-4)
                {
                }

                virtual ~Gradient() throw()
                {
                }

                void operator()(Array           &dFda,
                                Sequential<T>   &F,
                                param_type       x,
                                const Array     &aorg,
                                const Variables &vars,
                                const Flags     &used)
                {
                    assert(used.size()==aorg.size());
                    assert(dFda.size()==aorg.size());
                    const size_t nvar = aorg.size();
                    Wrapper      call = { nvar, &F, x &aorg, &vars  };
                    size_t      &i    = call.i;
                    for(;i>0;--i)
                    {
                        if(used[i])
                        {
                            dFda[i] = this->diff(call, x, h);
                        }
                        else
                        {
                            dFda[i] = 0;
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
                        
                        Sequential<T> &F    = *F_p;
                        const Array   &aorg = *aorg_p;
                        mutable_type  &ai   = aliasing::_(aorg[i]);
                        const_type     a0   = ai;
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

