

#include "y/ios/carrier.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/spec.hpp"
#include "y/exception.hpp"
#include "y/type/spec.hpp"
#include <iostream>

namespace upsylon
{
    
    namespace ios
    {
        carrier:: ~carrier() throw()
        {
            _bzset(bytes);
        }
        
        carrier:: carrier(const comms::shipping_style _style,
                          const comms::infrastructure _infra,
                          const std::type_info       &_clsid,
                          const size_t                _bytes) throw() :
        counted(),
        object(),
        style(_style),
        infra(_infra),
        clsid( rtti::of(_clsid) ),
        bytes(_bytes)
        {
            assert(bytes>0);
        }
        
        void carrier:: throw_missing_bytes() const
        {
            throw exception("ios::carrier::missing bytes for<%s>", clsid.text() );
        }
        
        
        std::ostream & operator<<(std::ostream &os, const carrier &self)
        {
            os << "carrier<" << self.clsid.name() << ">";
            os << " [" << comms::shipping_style_id(self.style) << ":" << comms::infrastructure_id(self.infra) << "]";
            return os;
        }
        
        const rtti  & carrier:: key() const throw()
        {
            return clsid;
        }
        
    }
    
}
