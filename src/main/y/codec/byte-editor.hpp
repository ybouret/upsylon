//! \file
#ifndef Y_CODEC_BYTE_EDITOR_INCLUDED
#define Y_CODEC_BYTE_EDITOR_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //! byte editor interface
    class byte_editor
    {
    public:
        //----------------------------------------------------------------------
        //
        // virtual interface
        //
        //----------------------------------------------------------------------
        virtual        ~byte_editor() throw();
        virtual uint8_t encode(uint8_t) throw() = 0;
        virtual uint8_t decode(uint8_t) throw() = 0;
        virtual void    reset()         throw() = 0;

        //----------------------------------------------------------------------
        //
        // non-virtual interface
        //
        //----------------------------------------------------------------------
        //! possible output=input
        void     encode_block(void *output,const void *input, size_t length) throw();

        //! possible output=input
        void     decode_block(void *output,const void *input, size_t length) throw();


    protected:
        explicit byte_editor() throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(byte_editor);
    };

}

#endif

