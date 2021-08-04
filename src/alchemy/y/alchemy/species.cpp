#include "y/alchemy/species.hpp"
#include "y/exception.hpp"
#include "y/randomized/bits.hpp"
#include <cmath>

namespace upsylon
{
    namespace Alchemy
    {
        
        Species:: ~Species() throw()
        {
        }
        
        

        const string & Species:: key() const throw()
        {
            return name;
        }

        void Species:: checkIndex() const
        {
            if(indx<=0) throw exception("%s.indx=0",*name);
        }

        double Species:: Concentration(randomized::bits &ran) throw()
        {
            return pow(10.0, ran.within<double>(min_exp10,max_exp10) );
        }


    }
    
}
