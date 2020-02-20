#include "y/information/modulation/delta.hpp"

namespace upsylon {

    namespace information {

        delta_modulation::~delta_modulation() throw() {}


        delta_modulation:: delta_modulation() throw() : last(0) {}

        void delta_modulation:: reset() throw() { last=0; }



    }

    namespace information {

        delta_encoder::  delta_encoder() throw() {}
        delta_encoder:: ~delta_encoder() throw() {}

        uint8_t delta_encoder:: fetch(uint8_t c) throw()
        {
            const uint8_t ans = c-last;
            last = c;
            return ans;
        }

        modulation * delta_encoder:: clone() const
        {
            delta_encoder *d = new delta_encoder();
            d->last = last;
            return d;
        }


    }

    namespace information {

        delta_decoder::  delta_decoder() throw() {}
        delta_decoder:: ~delta_decoder() throw() {}

        uint8_t delta_decoder:: fetch(uint8_t c) throw()
        {
            const uint8_t ans = c+last;
            last = ans;
            return ans;
        }

        modulation * delta_decoder:: clone() const
        {
            delta_decoder *d = new delta_decoder();
            d->last = last;
            return d;
        }
    }


}
