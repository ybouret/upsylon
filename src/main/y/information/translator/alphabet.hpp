
//! \file
#ifndef Y_INFORMATION_TRANSLATOR_ALPHABET_INCLUDED
#define Y_INFORMATION_TRANSLATOR_ALPHABET_INCLUDED 1

#include "y/information/qbits.hpp"

namespace upsylon {

    namespace information {

        
        //! base class for a dynamic alphabet
        class Alphabet
        {
        public:
            //--------------------------------------------------------------
            //
            // types and definitions
            //
            //--------------------------------------------------------------
            typedef size_t CodeType; //!< alias
            typedef size_t FreqType; //!< alias

            static const size_t   Bytes = 256;       //!< number of encoded byte   (0-255)
            static const CodeType EOS   = Bytes;     //!< Not Yet Transmitted Code (256)
            static const CodeType NYT   = EOS+1;     //!< End Of Stream            (257)
            static const size_t   Codes = NYT+1;     //!< maximum number of codes  (258)
            static const size_t   Alive = Codes-1;   //!< when all bytes are used, NYT is removed
            static const size_t   Nodes = 2*Alive+1; //!< maximum number of node for a tree
            static const size_t   Bits  = 16;        //!< maxium code bits
            
            //! a Char for a list of chars
            struct Char
            {
                CodeType symbol;    //!< original 0..Codes-1
                FreqType frequency; //!< current frequency
                size_t   code;      //!< current code
                size_t   bits;      //!< current bits
                Char    *next;      //!< for list
                Char    *prev;      //!< for list
                void    *priv;      //!< private data

                void emit( qbits &io ) const; //!< write code/bits
                void display() const;         //!< display info
            };

            //! a dynamic list of chars
            typedef core::list_of<Char> Chars;

            //--------------------------------------------------------------
            //
            // public methods
            //
            //--------------------------------------------------------------
            static const char *NameOf(const CodeType) throw(); //!< for information/graphviz


            virtual ~Alphabet()    throw(); //!< cleanup
            void    displayChars() const;   //!< for information
            bool    sameThan(const Alphabet &) const throw(); //!< to debug

        protected:
            Chars  chars;     //!< currently used char: 2..Alive
            Char  *eos;       //!< EOS
            Char  *nyt;       //!< NYT

            explicit Alphabet(); //!< setup
            void     emitAndUpdateByte(const uint8_t byte, qbits *io); //!< emit bits on io!=NULL, updated chars
            void     initialize() throw();                      //!< prepare chars
            void     rescaleFrequencies() throw();              //!< scale down used frequencies
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
            size_t       usedBytes; //!< 0..256
            size_t       charBytes; //!< bytes for character
            Char *       charNodes; //!< array of Char
        };

    }

}

#endif

