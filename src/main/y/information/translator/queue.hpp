//! \file

#ifndef Y_INFORMATION_TRANSLATOR_Q_INCLUDED
#define Y_INFORMATION_TRANSLATOR_Q_INCLUDED 1

#include "y/information/translator.hpp"
#include "y/sequence/list.hpp"

namespace upsylon {

    namespace information {

        class TranslatorQueue : public Translator, public list<char>
        {
        public:
            explicit TranslatorQueue()  throw();
            explicit TranslatorQueue(const size_t) throw();
            virtual ~TranslatorQueue()  throw();

            virtual bool query( char &C );
            virtual void store( char  C );
            virtual bool is_active() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(TranslatorQueue);
        };

    }

}

#endif

