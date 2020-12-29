

//! \file
#ifndef Y_FITTING_VARIABLE_REPLICA_INCLUDED
#define Y_FITTING_VARIABLE_REPLICA_INCLUDED 1

#include "y/mkl/fitting/variable.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            typedef intr_ptr<string,variable> shared_variable;

            class replica_variable : public variable
            {
            public:
                virtual ~replica_variable() throw();
                template <typename ID> inline
                explicit replica_variable(const ID              &id,
                                          const shared_variable &parent) :
                variable(id), source(parent)
                {
                }

                const shared_variable source;



            private:
                Y_DISABLE_COPY_AND_ASSIGN(replica_variable);
                virtual size_t get_index() const throw();
            };
        }
    }
}

#endif

