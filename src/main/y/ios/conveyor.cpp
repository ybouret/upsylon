
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
        
        conveyor:: conveyor(const comms::delivery    way,
                            const std::type_info    &usr,
                            const size_t             mov) throw() :
        mode(way),
        uuid(usr),
        move(mov)
        {
        }
        
        void conveyor:: missing_bytes() const
        {
            const type_spec &ts = type_spec::declare(uuid);
            throw exception("ios::conveyor(missing bytes for <%s>)", *(ts.name()) );
        }
            
        std::ostream & operator<<(std::ostream &os, const conveyor &c)
        {
            const type_spec &ts = type_spec::declare(c.uuid);
            os << "ios::conveyor<" << ts.name() << ":" << comms::delivery_id(c.mode) << ":#" << c.move << ">";
            return os;
        }


    }
    
}


