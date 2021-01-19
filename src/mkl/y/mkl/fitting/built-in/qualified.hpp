
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

                //______________________________________________________________
                //
                //
                //! build qualified variable names
                //
                //______________________________________________________________
                class qualified
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________
                    typedef vector<string,memory::pooled> strings; //!< alias

                    //__________________________________________________________
                    //
                    // methods
                    //__________________________________________________________
                    virtual ~qualified()         throw(); //!< cleanup
                    void     make(variables &vars) const; //!< make primary variables

                    //__________________________________________________________
                    //
                    // members
                    //__________________________________________________________
                    const strings names; //!< built by derived class
                    
                protected:
                    explicit qualified(const size_t nvar); //!< reserve space for nvar

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(qualified);
                };


            }

        }
    }

}

#endif
