
//! \file
#ifndef Y_INFORMATION_BASE64_ENCODER_INCLUDED
#define Y_INFORMATION_BASE64_ENCODER_INCLUDED 1

#include "y/information/translator/base64.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace Information {

        //! Base64 Encoder
        class Base64Encoder : public TranslatorQueue
        {
        public:
            const Base64::Mode mode; //!< flush behavior

            explicit Base64Encoder( const Base64::Mode = Base64::Pad ); //!< setup
            virtual ~Base64Encoder() throw(); //!< cleanup

            virtual const char *family() const throw(); //!< Base64:FMID
            virtual const char *name()   const throw(); //!< EncoderID

            virtual void reset() throw(); //!< reset all
            virtual void write(char C);   //!< write/encode
            virtual void flush();         //!< encode remaining
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Base64Encoder);

            enum Status
            {
                waitForByte1,
                waitForByte2,
                waitForByte3,
                waitForWrite
            };
            
            Status       status;
            const char  *table;
            char         source[3+1];
            char         target[4];

            void clear() throw();
            void emit(const bool flushing);
        };

    }

}

#endif

