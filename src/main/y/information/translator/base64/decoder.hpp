//! \file

#ifndef Y_INFORMATION_BASE64_DECODER_INCLUDED
#define Y_INFORMATION_BASE64_DECODER_INCLUDED 1

#include "y/information/translator/base64.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace Information {


        //! Base64 Decoder
        class Base64Decoder : public TranslatorQueue
        {
        public:

            explicit Base64Decoder();         //!< setup
            virtual ~Base64Decoder() throw(); //!< cleanup

            virtual const char *family() const throw(); //!< Base64:FMID
            virtual const char *name()   const throw(); //!< EncoderID

            virtual void reset() throw(); //!< reset all
            virtual void write(char C);   //!< write/decode
            virtual void flush();         //!< decode remaining

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
            void emit1();
            void emit2();
            void emit3();
            
        };

    }

}

#endif
