//! \file

#ifndef Y_TRANSLATOR_HUFFMAN_DECODER_INCLUDED
#define Y_TRANSLATOR_HUFFMAN_DECODER_INCLUDED 1

#include "y/information/translator/huffman.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            class Decoder : public Tree, public TranslatorQueue
            {
            public:
                explicit Decoder();
                virtual ~Decoder() throw();

                virtual void write(char C);
                virtual void flush();
                virtual void reset() throw();


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
