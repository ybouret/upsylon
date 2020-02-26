//! \file

#ifndef Y_INFORMATION_TRANSLATOR_ENCODING_Q_INCLUDED
#define Y_INFORMATION_TRANSLATOR_ENCODING_Q_INCLUDED 1

#include "y/information/translator/queue.hpp"
#include "y/information/qbits.hpp"

namespace upsylon {

    namespace information {

        //! implements the ios::istream part with qbits
        class TranslatorEncodingQueue : public TranslatorQueue
        {
        public:
            virtual ~TranslatorEncodingQueue() throw();

            virtual void writeBits(qbits &io, const char C)=0;     //!< manually write/update
            virtual void flushBits(qbits &io)              =0;     //!< manually flush

            virtual void write(char); //!< writeBits(Q,C) and compile
            virtual void flush();     //!< flushBits(Q)   and zfinish

        protected:
            explicit TranslatorEncodingQueue() throw();

            qbits Q; //!< output bits queue
        private:
            Y_DISABLE_COPY_AND_ASSIGN(TranslatorEncodingQueue);
        };

    }

}

#endif

