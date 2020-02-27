//! \file

#ifndef Y_INFORMATION_TRANSLATOR_Q_INCLUDED
#define Y_INFORMATION_TRANSLATOR_Q_INCLUDED 1

#include "y/information/translator.hpp"
#include "y/sequence/list.hpp"

namespace upsylon {

    namespace Information {

        //! implements the ios::istream part
        class TranslatorQueue : public Translator, public list<char>
        {
        public:
            explicit TranslatorQueue()             throw(); //!< setup
            explicit TranslatorQueue(const size_t) throw(); //!< setup with memory
            virtual ~TranslatorQueue()             throw(); //!< cleanup

            virtual bool query( char &C );    //!< from list front
            virtual void store( char  C );    //!< at list front
            virtual bool is_active() throw(); //!< size() != 0

        private:
            Y_DISABLE_COPY_AND_ASSIGN(TranslatorQueue);
        };

    }

}

#endif

