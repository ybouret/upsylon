
//! \file
#ifndef Y_INFORMATION_TRANSLATOR_ALPHABET_INCLUDED
#define Y_INFORMATION_TRANSLATOR_INCLUDED 1

#include "y/information/qbits.hpp"

namespace upsylon {

    namespace information {

        
        //! base class for a dynamic alphabet
        class Alphabet
        {
        public:
            typedef size_t CodeType;
            typedef size_t FreqType;

            static const size_t   Bytes = 256;     //!< number of encoded byte   (0-255)
            static const CodeType NYT   = Bytes;   //!< Not Yet Transmitted Code (256)
            static const CodeType EOS   = NYT+1;   //!< End Of Stream            (257)
            static const size_t   Codes = EOS+1;   //!< maximum number of codes  (258)
            static const size_t   Alive = Codes-1; //!< when all bytes are used, NYT is removed


            struct Char
            {
                CodeType symbol;    //!< original 0..Codes-1
                FreqType frequency; //!< current frequency
                size_t   code;      //!< current code
                size_t   bits;      //!< current bits
                Char    *next;      //!< for list
                Char    *prev;      //!< for list
                void emit( qbits &io ) const;
                void display() const;
            };

            typedef core::list_of<Char> Chars;

            static const char *NameOf(const CodeType code) throw();
            explicit Alphabet();
            virtual ~Alphabet() throw();

            void initialize() throw();
            

            Char * updateByte(const uint8_t byte, qbits &io) throw();
            Char * updateByte(const uint8_t byte) throw();
            void   rank(Char *chr) throw();

            void displayChars() const;

            bool sameThan( const Alphabet &other ) const throw(); //!< to debug

        protected:
            Chars  chars;     //!< currently used char: 2..Alive
            Char  *nyt;       //!< NYT
            Char  *eos;       //!< EOS
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
            size_t       usedBytes; //!< 0..256
            size_t       allocated;
            Char        *nodes;
        };

    }

}

#endif

