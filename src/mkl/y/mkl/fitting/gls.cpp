
#include "y/mkl/fitting/gls.hpp"

namespace upsylon
{

    namespace mkl
    {
        namespace fitting
        {

            

            gls:: ~gls() throw()
            {
                
            }

            gls:: gls(const bool verb) :
            gls_type(verb),
            solver( explode_type::default_solver::New() )
            {
            }

            
            
        }

    }

}

