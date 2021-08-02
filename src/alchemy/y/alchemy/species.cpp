#include "y/alchemy/species.hpp"
#include "y/exception.hpp"

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


    }
    
}
