

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

            typedef intr_ptr<string,variable> shared_variable; //!< alias

            //__________________________________________________________________
            //
            //
            //! replica variable, alias to another varialbe
            //
            //__________________________________________________________________
            class replica_variable : public variable
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~replica_variable() throw(); //!< cleanup

                //! setup with name and parent variable
                template <typename ID> inline
                explicit replica_variable(const ID              &id,
                                          const shared_variable &parent) :
                variable(id), source(parent)
                {
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const shared_variable source; //!< effective variable



            private:
                Y_DISABLE_COPY_AND_ASSIGN(replica_variable);
                virtual size_t operator*() const throw();
            };
        }
    }
}

#endif

