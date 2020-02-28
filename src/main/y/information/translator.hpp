//! \file
#ifndef Y_INFORMATION_TRANSLATOR_INCLUDED
#define Y_INFORMATION_TRANSLATOR_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"
#include "y/strfwd.hpp"

namespace upsylon {

    namespace Information {

        //! [enc|dec]oder
        class Translator : public ios::ostream, public ios::istream
        {
        public:
            static const char EncoderID[]; //!< "Encoder"
            static const char DecoderID[]; //!< "Decoder"
            
            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            virtual            ~Translator()   throw();     //!< cleanup
            virtual void        reset()        throw() = 0; //!< make a fresh start
            virtual const char *family() const throw() = 0; //!< type of algorithm
            virtual const char *name()   const throw() = 0; //!< [Enc|Dec]oder

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


            string toString(ios::istream &source, size_t *sourceLength = 0); //!< source -> string
            string toString(const void *buffer, const size_t length);        //!< block  -> string
            string toString(const char *text);                               //!< text   -> string
            string toString(const memory::ro_buffer &buffer);                //!< buffer -> string


        protected:
            explicit Translator() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Translator);
        };

    }

}

#endif

