
//! \file

#ifndef Y_TRANSLATOR_HUFFMAN_ENCODER_INCLUDED
#define Y_TRANSLATOR_HUFFMAN_ENCODER_INCLUDED 1

#include "y/information/translator/huffman.hpp"
#include "y/information/translator/encoding-queue.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            //! Huffman Encoder
            class Encoder : public Tree, public TranslatorEncodingQueue
            {
            public:
                virtual const char *family() const throw();         //!< FMID
                virtual const char *name() const throw();           //!< ENCODER
                explicit            Encoder();                      //!< initialize all
                virtual            ~Encoder() throw();              //!< cleanup
                virtual void        reset() throw();                //!< restart and clean all
                virtual void        writeBits(qbits &, const char); //!< manually write/update
                virtual void        flushBits(qbits &);             //!< manually flush
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            };

        }

    }
}

#endif

