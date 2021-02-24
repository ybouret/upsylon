
#include "y/concurrent/task/deeds.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace concurrent
    {
        shallow_deeds:: shallow_deeds() throw() : deeds()
        {
        }
        
        shallow_deeds:: ~shallow_deeds() throw()
        {
            release();
        }
        
        void shallow_deeds:: reserve(size_t n)
        {
            while(n-- > 0)
            {
                push_back( object::acquire1<deed>() );
            }
        }
        
        void shallow_deeds:: release() throw()
        {
            yield(object::release1<deed>);
        }

    }
    
}
#include "y/type/self-destruct.hpp"


namespace upsylon
{
    namespace concurrent
    {
        pending_deeds:: pending_deeds() throw() : deeds()
        {
        }
        
        pending_deeds:: ~pending_deeds() throw()
        {
            release();
        }
        
        void pending_deeds:: release() throw()
        {
            while(size)
            {
                deed *d = self_destruct::at( pop_back() );
                object::release1(d);
            }
        }
        
    }
    
}


