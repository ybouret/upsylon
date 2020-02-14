
//! \file
#ifndef Y_INFORMATION_HUFFMAN_DECODER_INCLUDED
#define Y_INFORMATION_HUFFMAN_DECODER_INCLUDED 1

#include "y/information/filter/huffman.hpp"
#include "y/information/filter/xqueue.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            //! decoder based on a context and a queue filter
            class Decoder : public Context, public filterXQ
            {
            public:
                explicit Decoder(const Mode m); //!< setup
                virtual ~Decoder() throw();     //!< cleanup

                virtual void reset() throw();   //!< restart context and free data
                virtual void write(char C);     //!< write to io and try to decode
                virtual void flush();           //!< do nothing

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Decoder);
                enum Flag
                {
                    wait_for_byte,
                    wait_for_bits
                };
                Flag    flag;
                Node   *curr;

                void decode();
                void onNewByte( const uint8_t u );
            };

        }

    }

}

#endif

