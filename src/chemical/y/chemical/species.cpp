

#include "y/chemical/species.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Species:: ~Species() throw()
        {
            aliasing::_(charge) = 0;
            aliasing::_(rating) = 0;
        }
        
        const string & Species:: key() const throw() { return name; }
 
        const char *Species:: Status(const size_t r) throw()
        {
            static const char *msg[] = {
                "waiting",
                "primary",
                "replica"
            };
            
            return msg[min_of<size_t>(r,2)];
        }

        bool Species:: isWorking() const throw()
        {
            return 0<rating;
        }

        bool Species:: isPrimary() const throw()
        {
            return 1==rating;
        }

        bool Species:: isReplica() const throw()
        {
            return 1<rating;
        }


    }
    
}

