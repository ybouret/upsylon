
#include "y/mkl/fitting/built-in/polynomial.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                __polynomial:: ~__polynomial() throw()
                {
                }

                void __polynomial:: setup()
                {
                    assert(0==vnames.size());
                    for(size_t d=0;d<=degree;++d)
                    {
                        const string id = var_id + vformat("%u",unsigned(d));
                        aliasing::_(vnames).push_back(id);
                    }
                }
                
                
                void __polynomial:: make(variables &vars) const
                {
                    for(size_t i=1;i<=coeffs;++i)
                    {
                        const string id = vnames[i];
                        if(!vars.has(id)) vars << id;
                    }
                }

            }
        }
    }
}

