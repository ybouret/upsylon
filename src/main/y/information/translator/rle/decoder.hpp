//! \file
#ifndef Y_INFORMATION_RLE_DECODER_INCLUDED
#define Y_INFORMATION_RLE_DECODER_INCLUDED 1

#include "y/information/translator/rle.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            class Decoder : public Parameters, public TranslatorQueue
            {
            public:
                virtual const char *name()   const throw();
                virtual const char *family() const throw();

                explicit Decoder();
                virtual ~Decoder() throw();

                virtual void write(char C);
                virtual void flush();
                virtual void reset() throw();

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
