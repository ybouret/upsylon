#include "y/information/modulation/delta.hpp"

namespace upsylon {

    namespace Information {

        DeltaModulation::~DeltaModulation() throw() {}


        DeltaModulation:: DeltaModulation() throw() : last(0) {}

        void DeltaModulation:: reset() throw() { last=0; }



    }

    namespace Information {


        Y_INFORMATION_MODULATION(DeltaEncoder);

        DeltaEncoder::  DeltaEncoder() throw() {}
        DeltaEncoder:: ~DeltaEncoder() throw() {}

        uint8_t DeltaEncoder:: fetch(uint8_t c) throw()
        {
            const uint8_t ans = c-last;
            last = c;
            return ans;
        }

        Modulation * DeltaEncoder:: clone() const
        {
            DeltaEncoder *d = new DeltaEncoder();
            d->last = last;
            return d;
        }


    }

    namespace Information {

        Y_INFORMATION_MODULATION(DeltaDecoder);


        DeltaDecoder::  DeltaDecoder() throw() {}
        DeltaDecoder:: ~DeltaDecoder() throw() {}

        uint8_t DeltaDecoder:: fetch(uint8_t c) throw()
        {
            const uint8_t ans = c+last;
            last = ans;
            return ans;
        }

        Modulation * DeltaDecoder:: clone() const
        {
            DeltaDecoder *d = new DeltaDecoder();
            d->last = last;
            return d;
        }
    }


}
