//! \file
#ifndef Y_CODEC_ASF_INCLUDED
#define Y_CODEC_ASF_INCLUDED 1

#include "y/core/list.hpp"
#include <iostream>

namespace upsylon
{
    struct ASF
    {
        typedef      size_t    CodeType;
        typedef      size_t    FreqType;
        static const size_t    NUM_CHARS  = 0xff;           //!< symbols an alphabet
        static const CodeType  NYT        = NUM_CHARS;      //!< Not Yet Transmitted
        static const CodeType  EOS        = NYT+1;          //!< End Of Stream
        static const size_t    ALPHABET_SIZE = NUM_CHARS+2; //!< chars+controls

        //! alphabet node
        struct Char
        {
            Char     *next;
            Char     *prev;
            CodeType  code;
            FreqType  freq;
            typedef core::list_of<Char> List;
            const char *text() const throw();
        };

        class Alphabet
        {
        public:
            Alphabet() throw();
            ~Alphabet() throw();

            void       reset() throw();
            Char       chars[ALPHABET_SIZE];
            Char::List active;
            Char      &nyt;
            Char      &eos;

            void display(std::ostream &os) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
        };
    };
}

#endif

