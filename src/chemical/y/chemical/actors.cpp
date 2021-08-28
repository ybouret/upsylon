#include "y/chemical/actors.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Actors:: ~Actors() throw()
        {
        }
        
        Actors:: Actors() : adb()
        {
        }
        
        void Actors:: operator()(const size_t nu, const Species &sp)
        {
            if(nu<=0) throw exception("cannot use 0*%s",*sp.name);
            
            const be_key key(sp);
            const Actor  act(nu,sp);
            if(!adb.insert(key,act))
            {
                throw exception("multiple actor '%s'",*sp.name);
            }
            
            aliasing::incr(sp.rating);
            
        }

        Actors::const_type & Actors:: bulk() const throw()
        {
            return adb;
        }
        
        bool Actors:: owns(const Species &sp) const throw()
        {
            const be_key key(sp);
            return adb.search(key) != NULL;
        }

        
    }
    
}


