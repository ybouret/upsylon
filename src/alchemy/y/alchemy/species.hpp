
//! \file

#ifndef Y_ALCHEMY_SPECIES_INCLUDED
#define Y_ALCHEMY_SPECIES_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    namespace alchemy
    {
        
        class species : public counted, public object
        {
        public:
            const string name;
            const int    z;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(species);
        };
        
    }
    
}

#endif

