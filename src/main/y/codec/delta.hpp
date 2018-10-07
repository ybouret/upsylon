//! \file
#ifndef Y_CODEC_DELTA_INCLUDED
#define Y_CODEC_DELTA_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    class delta_encoding
    {
    public:
        explicit delta_encoding() throw();
        virtual ~delta_encoding() throw();
        void reset() throw();

        uint8_t encode(uint8_t);
        uint8_t decode(uint8_t);

    private:
        uint8_t last;
        Y_DISABLE_COPY_AND_ASSIGN(delta_encoding);
    };
}

#endif

