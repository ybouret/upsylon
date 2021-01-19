
#include "y/mkl/fitting/built-in/polynomial.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                namespace bulk
                {
                    polynomial:: ~polynomial() throw()
                    {
                    }

                    void polynomial:: setup()
                    {
                        assert(0==names.size());
                        for(size_t d=0;d<=degree;++d)
                        {
                            const string id = var_id + vformat("%u",unsigned(d));
                            aliasing::_(names).push_back(id);
                        }
                    }

                }

            }
        }
    }
}

