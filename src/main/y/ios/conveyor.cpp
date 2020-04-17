
#include "y/ios/conveyor.hpp"
#include "y/exception.hpp"
#include "y/string.hpp"

namespace upsylon {
    
    namespace ios
    {
        conveyor:: ~conveyor() throw()
        {
        }
        
        conveyor:: conveyor(const comm_mode c) throw() : comm(c)
        {
        }
        
        void conveyor:: missing_bytes(const string &type_name) const
        {
            throw exception("ios::conveyor(missing bytes for <%s>)", *type_name );
        }
            

    }
    
}


