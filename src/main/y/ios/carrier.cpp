

#include "y/ios/carrier.hpp"
#include "y/exception.hpp"
#include <iostream>

namespace upsylon
{
    
    namespace ios
    {
        carrier:: ~carrier() throw()
        {
        }

        const char carrier:: CLSID[] = "ios::carrier";
        
        carrier:: carrier(const comms::shipping_style _style,
                          const comms::infrastructure _infra,
                          const std::type_info       &_clsid) :
        counted(),
        object(),
        style(_style),
        infra(_infra),
        clsid( rtti::of(_clsid) )
        {
            if(clsid.size<=0) throw exception("%s(unset <%s>.size)", CLSID, clsid.text() );
        }
        
        void carrier:: throw_missing_bytes() const
        {
            throw exception("%s(missing bytes for<%s>)", CLSID, clsid.text() );
        }

       
        
        
        std::ostream & operator<<(std::ostream &os, const carrier &self)
        {
            os << carrier::CLSID << '<' << self.clsid.name() << '>';
            os << " [" << comms::shipping_style_id(self.style) << ":" << comms::infrastructure_id(self.infra) << "]";
            return os;
        }
        
        const rtti  & carrier:: key() const throw()
        {
            return clsid;
        }
        
    }
    
}
