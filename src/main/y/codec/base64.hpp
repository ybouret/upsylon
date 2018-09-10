//! \file

#ifndef Y_CODEC_BASE64_INCLUDED
#define Y_CODEC_BASE64_INCLUDED

#include "y/ios/codec.hpp"

namespace upsylon
{
    namespace ios
    {
        struct base64
        {

            static const char encode_std[64];
            static const char encode_url[64];

            class encoder : public q_codec
            {
            public:
                enum status
                {
                    wait0,
                    wait1,
                    wait2
                };

                explicit encoder(bool use_pad=true, bool use_url=false) throw();
                virtual ~encoder() throw();

                virtual void reset() throw();
                virtual void write( char C );
                virtual void flush();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(encoder);
                status      flag;     //!< where are we
                uint8_t     data[4];  //!< ready
                const char *table;
                const bool  pad;
                void     emit(const size_t n);
            };

        };
    }
}

#endif


