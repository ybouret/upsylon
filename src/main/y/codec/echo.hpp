//! \file
#ifndef Y_CODEC_ECHO_INCLUDED
#define Y_CODEC_ECHO_INCLUDED 1

#include "y/codec/byte-editor.hpp"

namespace upsylon {

    //! delta encoding method
    class echo_encoding
    {
    public:
        explicit echo_encoding() throw(); //!< initialize
        virtual ~echo_encoding() throw(); //!< desctructor


        virtual uint8_t encode(uint8_t) throw(); //!< encode a byte
        virtual uint8_t decode(uint8_t) throw(); //!< decode a byte
        virtual void    reset()         throw(); //!< reset
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(echo_encoding);
    };
}

#endif

