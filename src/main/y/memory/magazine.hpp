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
            typedef singleton< magazine<T> > singleton_type;
            typedef tight::supply_of<T>      supply_type;

            Y_SINGLETON_DECL(magazine);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(magazine);
            inline explicit magazine() : singleton_type(), supply_type()
            {
            }

            inline virtual ~magazine() throw()
            {
            }
        };
    }
    
}

#endif
