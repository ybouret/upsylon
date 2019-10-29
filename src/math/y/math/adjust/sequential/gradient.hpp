

#ifndef Y_MATH_ADJUST_SEQUENTIAL_GRADIENT_INCLUDED
#define Y_MATH_ADJUST_SEQUENTIAL_GRADIENT_INCLUDED 1

#include "y/math/adjust/sequential.hpp"
#include "y/math/fcn/derivatives.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            //==================================================================
            //
            //
            //! Gradient w.r.t parameters
            //
            //
            //==================================================================
            template <typename T>
            class Gradient : public derivative<T>
            {
            public:
                T h; //!< scaling
                
                //! setup with default scaling
                inline explicit Gradient() : derivative<T>(), h(1e-4)
                {
                }

                //! cleanup
                inline virtual ~Gradient() throw()
                {
                    h=0;
                }

                //! take the gradient for the declared used variables
                inline void operator()(addressable<T>         &dFda,
                                       Sequential<T>          &F,
                                       T                       x,
                                       const accessible<T>    &aorg,
                                       const Variables        &vars,
                                       const accessible<bool> &used)
                {
                    assert(used.size()==aorg.size());
                    assert(dFda.size()==aorg.size());

                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                    for(size_t j=dFda.size();j>0;--j)
                    {
                        dFda[j] = 0;
                    }

                    //----------------------------------------------------------
                    // prepare wrapper
                    //----------------------------------------------------------
                    Wrapper              call = { 0, &F, x, &aorg, &vars  };
                    size_t              &indx = call.i;
                    size_t               nvar = vars.size();
                    const accessible<T> &a    = aorg;
                    
                    //----------------------------------------------------------
                    // lover over variables
                    //----------------------------------------------------------
                    for( Variables::const_iterator v = vars.begin();nvar>0;--nvar,++v)
                    {
                        indx = (*v)->index();
                        if(used[indx])
                        {
                            dFda[indx] = this->diff(call, a[indx], h);
                        }
                    }
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gradient);
                struct Wrapper
                {
                    size_t                 i;
                    Sequential<T>         *F_p;
                    T                      x;
                    const accessible<T>   *aorg_p;
                    const Variables       *vars_p;

                    inline T operator()( const T a )
                    {
                        assert(i>0);
                        assert(F_p);
                        assert(aorg_p);
                        assert(vars_p);
                        
                        Sequential<T>        &F    = *F_p;
                        const accessible<T>  &aorg = *aorg_p;
                        T                    &ai   = aliasing::_(aorg[i]);
                        const T               a0   = ai;
                        try {
                            ai = a;
                            const T f = F.start(x,aorg,*vars_p);
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

