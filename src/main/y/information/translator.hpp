//! \file
#ifndef Y_INFORMATION_TRANSLATOR_INCLUDED
#define Y_INFORMATION_TRANSLATOR_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"
#include "y/strfwd.hpp"

namespace upsylon {

    namespace information {

        //! [enc|dec]oder
        class Translator : public ios::ostream, public ios::istream
        {
        public:
            virtual ~Translator() throw();

            //! process/flush without touching state
            size_t process(ios::ostream &target,
                           ios::istream &source,
                           size_t *sourceLength = 0);
            
            virtual void reset() throw() = 0;
            
            static size_t Fibonnaci(const string &dataName, const uint8_t a, const uint8_t b);

            size_t testFibonnaci(const string &encoded,
                                 const string &fibonnaci,
                                 const uint8_t a,
                                 const uint8_t b,
                                 size_t       &ngen);

        protected:
            explicit Translator() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Translator);
        };

    }

}

#endif

