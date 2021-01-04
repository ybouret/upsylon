
//! \file

#ifndef Y_FITTING_SEQUENTIAL_NUM_GRAD_INCLUDED
#define Y_FITTING_SEQUENTIAL_NUM_GRAD_INCLUDED 1

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
            //! sequential gradient interface
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class sequential_num_grad :
            //public sequential_gradient<ABSCISSA,ORDINATE>,
            public derivative<ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sequential<ABSCISSA,ORDINATE>          sequential_type; //!< alias
                //typedef sequential_gradient<ABSCISSA,ORDINATE> sequential_grad; //!< alias
                typedef derivative<ORDINATE>                   derivative_type; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit sequential_num_grad() :
                //sequential_grad(),
                derivative_type(), h() {}

                inline virtual ~sequential_num_grad() throw() {}

                //______________________________________________________________
                //
                // members
                //______________________________________________________________

                vector<ORDINATE> h; //!< scaling to evaluate parameters

            private:
                Y_DISABLE_COPY_AND_ASSIGN(sequential_num_grad);

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
                                     sequential_type            &F,
                                     const ABSCISSA              X,
                                     const accessible<ORDINATE> &aorg,
                                     const variables            &vars,
                                     const accessible<bool>     &used)
                {

                    // initialize
                    wrapper func = { 0, &F, &X, &aorg, &vars }; // phony 1D function
                    size_t  nvar = vars.size();                 // number of variables
                    
                    // run on variables
                    for(variables::const_iterator it=vars.begin();nvar>0;--nvar,++it)
                    {
                        const size_t    ia = (func.ia = (**it).get_index());
                        if(used[ia])
                        {
                            dFdA[ia] = this->diff(func,aorg[ia],1e-4);
                        }
                    }
                }
            };

        }
    }
}

#endif
