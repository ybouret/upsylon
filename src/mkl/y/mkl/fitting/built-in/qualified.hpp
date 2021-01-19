
//! \file


#ifndef Y_FITTING_BUILT_QUALIFIED_INCLUDED
#define Y_FITTING_BUILT_QUALIFIED_INCLUDED 1

#include "y/mkl/fitting/variables.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                class qualified
                {
                public:
                    typedef vector<string,memory::pooled> strings;

                    virtual ~qualified() throw();
                    void make(variables &vars) const; //!< make primary variables

                    
                    const strings names;
                    
                protected:
                    explicit qualified(const size_t nvar);

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(qualified);
                };


            }

        }
    }

}

#endif
