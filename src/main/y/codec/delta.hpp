//! \file
#ifndef Y_CODEC_DELTA_INCLUDED
#define Y_CODEC_DELTA_INCLUDED 1

#include "y/codec/byte-editor.hpp"

namespace upsylon
{
    //! delta encoding method
    class delta_encoding : public byte_editor
    {
    public:
        explicit delta_encoding() throw(); //!< initialize
        virtual ~delta_encoding() throw(); //!< destructor


        virtual uint8_t encode(uint8_t) throw();           //!< encode a byte
        virtual uint8_t decode(uint8_t) throw();           //!< decode a byte
        virtual void    reset()         throw();           //!< reset
        
    private:
        uint8_t last;
        Y_DISABLE_COPY_AND_ASSIGN(delta_encoding);
    };
}

#endif

