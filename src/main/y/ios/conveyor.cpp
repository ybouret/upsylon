
#include "y/ios/conveyor.hpp"
#include "y/exception.hpp"
#include "y/string.hpp"
#include "y/type/spec.hpp"

namespace upsylon {
    
    namespace ios
    {
        conveyor:: ~conveyor() throw()
        {
        }
        
        conveyor:: conveyor(const comms::manner   how,
                            const std::type_info &usr) throw() :
        comms_manner(how),
        tid(usr)
        {
        }
        
        void conveyor:: missing_bytes() const
        {
            const type_spec &ts = type_spec::declare(tid);
            throw exception("ios::conveyor(missing bytes for <%s>)", *(ts.name()) );
        }
            
        std::ostream & operator<<(std::ostream &os, const conveyor &c)
        {
            const type_spec &ts = type_spec::declare(c.tid);
            os << "ios::conveyor<" << ts.name() << ":" << comms::manner_text(c.comms_manner) << ">";
            return os;
        }


    }
    
}


