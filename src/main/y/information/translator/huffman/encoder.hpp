
//! \file

#ifndef Y_TRANSLATOR_HUFFMAN_ENCODER_INCLUDED
#define Y_TRANSLATOR_HUFFMAN_ENCODER_INCLUDED 1

#include "y/information/translator/huffman.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            //! Huffman Encoder
            class Encoder : public Tree, public TranslatorQueue
            {
            public:
                virtual const char *family() const throw(); //!< FMID
                virtual const char *name() const throw();   //!< ENCODER
                explicit            Encoder();              //!< initialize all
                virtual            ~Encoder() throw();      //!< cleanup
                virtual void        write(char C);          //!< emit according to tree and update
                virtual void        flush();                //!< emit EOS
                virtual void        reset() throw();        //!< restart and clean all

                virtual void writeBits(qbits &io, const char C);     //!< manually write/update
                virtual void flushBits(qbits &io);                   //!< manually flush
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
                qbits Q;
            };

        }

    }
}

#endif

