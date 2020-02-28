//! \file

#ifndef Y_TRANSLATOR_HUFFMAN_DECODER_INCLUDED
#define Y_TRANSLATOR_HUFFMAN_DECODER_INCLUDED 1

#include "y/information/translator/huffman.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace Information {

        namespace Huffman {

            //! decoder for Huffman
            class Decoder : public Tree, public TranslatorQueue
            {
            public:
                virtual const char *family() const throw(); //!< FMID
                virtual const char *name() const throw();   //!< DECODER
                explicit            Decoder();              //!< setup, wit for byte
                virtual            ~Decoder() throw();      //!< cleanup
                virtual void        write(char C);          //!< process bits
                virtual void        flush();                //!< do nothing
                virtual void        reset() throw();        //!< restart all

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Decoder);
                IOBits Q;
                enum Flag
                {
                    waitForBits,
                    waitForByte
                };
                Flag  flag;
                Node *curr;
                void emitByte( const uint8_t byte );
                
            };

        }

    }
}

#endif
