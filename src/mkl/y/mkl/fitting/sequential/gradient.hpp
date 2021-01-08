
//! \file

#ifndef Y_FITTING_SEQUENTIAL_GRADIENT_INCLUDED
#define Y_FITTING_SEQUENTIAL_GRADIENT_INCLUDED 1

#include "y/mkl/fitting/v-gradient.hpp"
#include "y/mkl/fcn/derivative.hpp"
#include "y/momentary/value.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! numerical sequential gradient
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class sequential_gradient:
            public v_gradient<ABSCISSA,ORDINATE>,
            public derivative<ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sequential<ABSCISSA,ORDINATE> sequential_type; //!< alias
                typedef v_gradient<ABSCISSA,ORDINATE> v_gradient_type; //!< alias
                typedef derivative<ORDINATE>          derivative_type; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                inline explicit sequential_gradient() :
                v_gradient_type(),
                derivative_type(),
                F(NULL),
                h( ORDINATE(1e-4) ) {}

                inline virtual ~sequential_gradient() throw() {}

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                sequential_type *F; //!< sequential function
                ORDINATE         h; //!< scaling to evaluate parameters

            private:
                Y_DISABLE_COPY_AND_ASSIGN(sequential_gradient);

                struct wrapper
                {
                    size_t                      ia;
                    sequential_type            *F_;
                    const ABSCISSA             *X_;
                    const accessible<ORDINATE> *aorg_;
                    const variables            *vars_;

                    inline ORDINATE operator()(const ORDINATE a)
                    {
                        assert(ia>0); assert(F_); assert(X_); assert(aorg_); assert(vars_);
                        const accessible<ORDINATE> &aorg = *aorg_;
                        momentary_value<ORDINATE>   link( aliasing::_( aorg[ia]), a );
                        return F_->start(*X_,aorg,*vars_);
                    }
                };

                virtual void compute(addressable<ORDINATE>      &dFdA,
                                     const ABSCISSA              X,
                                     const accessible<ORDINATE> &aorg,
                                     const variables            &vars,
                                     const accessible<bool>     &used)
                {

                    //__________________________________________________________
                    //
                    // initialize
                    //__________________________________________________________
                    assert(F);
                    wrapper func = { 0, F, &X, &aorg, &vars }; // phony 1D function
                    size_t  nvar = vars.size();                 // number of variables

                    //__________________________________________________________
                    //
                    // run on variables
                    //__________________________________________________________
                    for(variables::const_iterator it=vars.begin();nvar>0;--nvar,++it)
                    {
                        const size_t    ia = (func.ia = ***it);
                        if(used[ia])
                        {
                            dFdA[ia] = this->diff(func,aorg[ia],h);
                        }
                    }
                }
            };

        }
    }
}

#endif
