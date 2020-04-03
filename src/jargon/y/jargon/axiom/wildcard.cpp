
#include "y/jargon/axiom/wildcard.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Wildcard:: ~Wildcard() throw()
        {
           
        }
        

        string * Wildcard::MakeTag( const char mark, const string &name )
        {
            const string id = mark + name;
            return Tags::Make(id);
        }
        
        const char *   Wildcard:: vizStyle() const throw()
        {
            return "bold";
        }

        
        
    }
    
}

