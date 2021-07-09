
//! \file

#ifndef Y_MKL_FITTING_CYCLE_INCLUDED
#define Y_MKL_FITTING_CYCLE_INCLUDED 1

#include "y/mkl/fitting/least-squares.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace fitting
        {
            //__________________________________________________________________
            //
            //
            //! wrapper to perform least squares cycles
            //
            //__________________________________________________________________
            template <typename LS_TYPE>
            class cycles
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef LS_TYPE                           ls_type;         //!< alias
                typedef typename ls_type::sample_api_type sample_api_type; //!< alias
                typedef typename ls_type::ordinate        ordinate;        //!< alias
                typedef addressable<ordinate>             parameters;      //!< alias
                typedef addressable<bool>                 booleans;        //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~cycles() throw() { reset(); }

                //! setup
                explicit cycles(  sample_api_type     &user_data,
                                parameters            &user_aorg,
                                booleans              &user_used,
                                parameters            &user_aerr) throw() :
                data(user_data),
                aorg(user_aorg),
                used(user_used),
                aerr(user_aerr),
                number(0)
                {
                }


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! reset before a new search profile
                inline void reset() throw()
                {
                    aliasing::zero(number);
                }

                //! call/check fit with variable choice
                template <typename FUNC, typename VARS>
                void operator()(ls_type &ls, FUNC &F, const VARS &choice)
                {
                    // setup
                    const string id(choice);
                    data.vars.only_on(used,id);
                    aliasing::incr(number);

                    // call fit
                    if( !ls.fit(data,F,aorg,used,aerr) )
                        throw exception("%s: failed to fit with '%s' @ cycle=%lu", *data.name, *id, number);

                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                sample_api_type       &data;    //!< sample API to use
                parameters            &aorg;    //!< user's parameters
                booleans              &used;    //!< user's flags
                parameters            &aerr;    //!< user's error on parameters
                const unsigned long    number;  //!< number of cycles so far

            private:
                Y_DISABLE_COPY_AND_ASSIGN(cycles);
            };
        }

    }
}

#endif

