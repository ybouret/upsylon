

#include "y/mkl/fitting/built-in/qualified.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                qualified:: ~qualified() throw()
                {
                }

                qualified:: qualified(const size_t nvar) :
                names(nvar,as_capacity)
                {
                }
                
                void qualified:: make(variables &vars) const
                {
                    const size_t nvar = names.size();
                    for(size_t i=1;i<=nvar;++i)
                    {
                        const string &id = names[i];
                        if(!vars.has(id)) vars << id;
                    }
                }
                
            }
        }
    }
}

