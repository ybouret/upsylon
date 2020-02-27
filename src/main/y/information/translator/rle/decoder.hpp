//! \file
#ifndef Y_INFORMATION_RLE_DECODER_INCLUDED
#define Y_INFORMATION_RLE_DECODER_INCLUDED 1

#include "y/information/translator/rle.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace Information {

        namespace RLE {

            //! RLE Decoder
            class Decoder : public Parameters, public TranslatorQueue
            {
            public:
                virtual const char *name()   const throw(); //!< DecoderID
                virtual const char *family() const throw(); //!< FMID

                explicit Decoder();          //!< setup
                virtual ~Decoder() throw();  //!< cleaup

                virtual void write(char C);   //!< decode next char
                virtual void flush();         //!< should do nothing
                virtual void reset() throw(); //!< fresh start

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Decoder);
                enum Status
                {
                    waitForDirective,
                    waitForRepeating,
                    waitForDifferent
                };
                Status status;
                size_t count;
                
            };

        }
    }
}


#endif
