#ifndef Y_AQUA_SPECIES_INCLUDED
#define Y_AQUA_SPECIES_INCLUDED 1

#include "y/string.hpp"

namespace upsylon {
 
    namespace Aqua
    {
        
        class Species
        {
        public:
            virtual ~Species() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }
    
}
#endif

