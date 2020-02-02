//! \file

#ifndef Y_CODEC_NESTED_INCLUDED
#define Y_CODEC_NESTED_INCLUDED 1

#include "y/codec/echo.hpp"
#include "y/ios/codec.hpp"

namespace upsylon {

    template <typename CODEC>
    class nested_   : public ios::codec
    {
    public:
        inline virtual ~nested_() throw() {}

        inline virtual void flush()
        {
            code.flush();
        }

        inline virtual void reset() throw()
        {
            code.reset();
        }


        inline virtual void store(const char C)
        {
            code.store(C);
        }


    protected:
        CODEC  code;

        inline explicit nested_() : code() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(nested_);
    };

    template <
    typename ENCODER,
    typename EDITOR=echo_encoding>
    class nested_encoder : public nested_<ENCODER>
    {
    public:
        inline explicit nested_encoder() : nested_<ENCODER>(), edit() {}
        inline virtual ~nested_encoder() throw() {}

        //! ostream interface of codec
        inline virtual void write(const char C)
        {
            this->code.write( edit.encode(C) );
        }

        //! istream interface of codec
        inline virtual bool query(char &C)
        {
            return this->code.query(C);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(nested_encoder);
        EDITOR edit;
    };

    template <
    typename DECODER,
    typename EDITOR=echo_encoding>
    class nested_decoder : public nested_<DECODER>
    {
    public:
        inline explicit nested_decoder() : nested_<DECODER>(), edit() {}
        inline virtual ~nested_decoder() throw() {}

        //! ostream interface of codec
        inline virtual void write(const char C)
        {
            this->code.write( C );
        }

        


    private:
        Y_DISABLE_COPY_AND_ASSIGN(nested_decoder);
        EDITOR edit;
    };

}

#endif

