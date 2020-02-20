
#ifndef Y_INFORMATION_TRANSLATOR_TREE_INCLUDED
#define Y_INFORMATION_TRANSLATOR_TREE_INCLUDED 1

#include "y/information/translator/alphabet.hpp"

namespace upsylon {

    namespace information {

        class TranslatorTree : public Alphabet
        {
        public:
            virtual ~TranslatorTree() throw();

            explicit TranslatorTree() throw();

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(TranslatorTree);
        };

    }
}

#endif

