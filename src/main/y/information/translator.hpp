//! \file

#ifndef Y_INFORMATION_TRANSLATOR_INCLUDED
#define Y_INFORMATION_TRANSLATOR_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace information {


        class Translator : public ios::ostream, public ios::istream
        {
        public:
            virtual ~Translator() throw();
            
        protected:
            explicit Translator() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Translator);
        };

    }

}

#endif

