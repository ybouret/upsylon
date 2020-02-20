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
            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            virtual            ~Translator()   throw();     //!< cleanup
            virtual void        reset()        throw() = 0; //!< make a fresh start
            virtual const char *name()   const throw() = 0; //!< the name

            //------------------------------------------------------------------
            //
            // non-virtual interface
            //
            //------------------------------------------------------------------

            //! process/flush without touching state
            size_t process(ios::ostream &target,
                           ios::istream &source,
                           size_t *sourceLength = 0);
            
            //! create a Fibonacci file
            static size_t Fibonacci(const string &fileName, const uint8_t a, const uint8_t b);

            //! test the codec with an optional decoder
            void testCODEC(const string &fileName,
                           const string &compName,
                           const string &backName,
                           Translator   *decoder);

        protected:
            explicit Translator() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Translator);
        };

    }

}

#endif

