
//! \file

#ifndef Y_TRANSLATOR_SHANNON_FANO_DECODER_INCLUDED
#define Y_TRANSLATOR_SHANNON_FANO_DECODER_INCLUDED 1

#include "y/information/translator/shannon-fano.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            //! decoder for Huffman
            class Decoder : public Tree, public TranslatorQueue
            {
            public:
                explicit     Decoder();         //!< setup, wit for byte
                virtual     ~Decoder() throw(); //!< cleanup
                virtual void write(char C);     //!< process bits
                virtual void flush();           //!< do nothing
                virtual void reset() throw();   //!< restart all

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Decoder);
                qbits Q;
                enum Flag
                {
                    waitForByte,
                    waitForBits
                };
                Flag  flag;
                Node *curr;
                void emitByte( const uint8_t byte );

            };

        }

    }
}

#endif
