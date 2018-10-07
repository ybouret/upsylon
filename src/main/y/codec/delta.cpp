#include "y/codec/delta.hpp"

namespace upsylon {

    delta_encoding::~delta_encoding() throw() {}


    delta_encoding:: delta_encoding() throw() : last(0) {}

    void delta_encoding:: reset() throw() { last=0; }


    uint8_t delta_encoding:: encode(uint8_t c)
    {
        const uint8_t ans = c-last;
        last = c;
        return ans;
    }

    uint8_t delta_encoding:: decode(uint8_t c)
    {
        const uint8_t ans = c+last;
        last = ans;
        return ans;
    }


}
