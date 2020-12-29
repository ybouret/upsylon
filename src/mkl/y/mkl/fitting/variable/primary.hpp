
//! \file
#ifndef Y_FITTING_VARIABLE_PRIMARY_INCLUDED
#define Y_FITTING_VARIABLE_PRIMARY_INCLUDED 1

#include "y/mkl/fitting/variable.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            class primary_variable : public variable
            {
            public:
                virtual ~primary_variable() throw();
                template <typename ID> inline
                explicit primary_variable(const ID &id, const size_t the_index) :
                variable(id), indx( check_index(the_index) )
                {
                }

                
                const size_t indx;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(primary_variable);
                size_t check_index(const size_t the_index) const;
                virtual size_t get_index() const throw();
            };
        }
    }
}

#endif

