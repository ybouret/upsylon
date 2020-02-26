

//! \file

#ifndef Y_TRANSLATOR_SHANNON_FANO_ENCODER_INCLUDED
#define Y_TRANSLATOR_SHANNON_FANO_ENCODER_INCLUDED 1

#include "y/information/translator/shannon-fano.hpp"
#include "y/information/translator/encoding-queue.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            //! ShannonFano Encoder
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

