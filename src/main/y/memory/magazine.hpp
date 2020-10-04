#ifndef Y_MEMORY_MAGAZINE_INCLUDED
#define Y_MEMORY_MAGAZINE_INCLUDED 1

#include "y/memory/tight/supply.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon
{
    
    namespace memory
    {
        
        template <typename T>
        class magazine : public singleton< magazine<T> >, public tight::supply_of<T>
        {
        public:
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(magazine);
            inline explicit magazine()
            {
            }
            inline virtual ~magazine() throw()
            {
            }
        };
    }
    
}

#endif
