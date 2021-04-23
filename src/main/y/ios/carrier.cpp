

#include "y/ios/carrier.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/spec.hpp"
#include "y/exception.hpp"
#include "y/type/spec.hpp"

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
        clsid(_clsid),
        bytes(_bytes)
        {
            assert(bytes>0);
        }
        
        void carrier:: throw_missing_bytes() const
        {
            const string &id = type_name_for(clsid);
            throw exception("ios::carrier::missing bytes for<%s>", *id);
        }

    }
    
}
