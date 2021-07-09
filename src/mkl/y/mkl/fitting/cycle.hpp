
//! \file

#ifndef Y_MKL_FITTING_CYCLE_INCLUDED
#define Y_MKL_FITTING_CYCLE_INCLUDED 1

#include "y/mkl/fitting/least-squares.hpp"

namespace upsylon
{
    namespace mkl
    {

#define Y_MKL_FITTING_CYCLE_ECHO(TYPE) typedef typename ls_type::TYPE TYPE
        template <typename LS_TYPE>
        class cycles
        {
        public:
            typedef LS_TYPE ls_type;
            Y_MKL_FITTING_CYCLE_ECHO(sample_api_type);
            Y_MKL_FITTING_CYCLE_ECHO(ordinate);
            typedef addressable<ordinate> parameters;
            typedef addressable<bool>     booleans;

            inline virtual ~cycles() throw() {}

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

            sample_api_type       &data;
            parameters            &aorg;
            booleans              &used;
            parameters            &aerr;
            const unsigned long    number;

            template <typename FUNC, typename VARS>
            void operator()(ls_type &ls, FUNC &F, const VARS &choice)
            {
                // setup
                const string id(choice);
                data.vars.only_on(used,id);
                aliasing::incr(number);

                // call fit
                if( ls.fit(data,F,aorg,used,aerr) )
                {

                }
                else
                {
                    throw exception("%s: failed to fit with '%s'", *data.name, *id);
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(cycles);
        };

    }
}

#endif

