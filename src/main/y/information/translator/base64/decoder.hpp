//! \file

#ifndef Y_INFORMATION_BASE64_DECODER_INCLUDED
#define Y_INFORMATION_BASE64_DECODER_INCLUDED 1

#include "y/information/translator/base64.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace Information {


        class Base64Decoder : public TranslatorQueue
        {
        public:

            explicit Base64Decoder(   );
            virtual ~Base64Decoder() throw();

            virtual const char *family() const throw(); //!< Base64:FMID
            virtual const char *name()   const throw(); //!< EncoderID

            virtual void reset() throw();
            virtual void write(char C);
            virtual void flush();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Base64Decoder);
            enum Status
            {
                waitForChar1,
                waitForChar2,
                waitForChar3,
                waitForChar4,
                waitForWrite
            };
            Status status;
            char   source[4];
            char   target[3+1];

            void clear() throw();
            void emit();
        };

    }

}

#endif
