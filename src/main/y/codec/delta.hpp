//! \file
#ifndef Y_CODEC_DELTA_INCLUDED
#define Y_CODEC_DELTA_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! delta encoding method
    class delta_encoding
    {
    public:
        explicit delta_encoding() throw(); //!< initialize
        virtual ~delta_encoding() throw(); //!< desctructor
        void reset() throw();              //!< reset

        uint8_t encode(uint8_t);           //!< encode a byte
        uint8_t decode(uint8_t);           //!< decode a byte

    private:
        uint8_t last;
        Y_DISABLE_COPY_AND_ASSIGN(delta_encoding);
    };
}

#endif

