//! \file
#ifndef Y_CODEC_ASF_INCLUDED
#define Y_CODEC_ASF_INCLUDED 1

#include "y/core/list.hpp"
#include "y/ios/ostream.hpp"
#include "y/codec/iobits.hpp"

#include <iostream>
namespace upsylon
{
    struct ASF
    {
        typedef      size_t    CodeType;
        typedef      size_t    FreqType;
        static const size_t    NUM_CHARS     = 0xff;           //!< symbols an alphabet
        static const CodeType  NYT           = NUM_CHARS;      //!< Not Yet Transmitted
        static const CodeType  EOS           = NYT+1;          //!< End Of Stream
        static const size_t    ALPHABET_SIZE = NUM_CHARS+2; //!< chars+controls
        struct Char;

        struct Node
        {
            Node       *left;
            Node       *right;
            FreqType    freq; //!< total frequency
            const Char *ch;   //!< terminal node
            CodeType    code; //!< prefix code
            FreqType    bits; //!< prefix bits
            void        viz( ios::ostream & ) const;
        };

        //! alphabet node
        struct Char
        {
            Char             *next;
            Char             *prev;
            CodeType          byte;
            FreqType          freq;
            mutable CodeType  code;
            mutable FreqType  bits;
            mutable FreqType  part;
            typedef core::list_of<Char> List;
            const char *text() const throw();
        };
        
        class Alphabet
        {
        public:
            Alphabet();
            ~Alphabet() throw();

            void       reset() throw();
            Char::List active;
            bool       first;
            Char      *chars;
            Node      *nodes;
            Char      *nyt;
            Char      *eos;

            void display(std::ostream &os) const;

            void add(const char C) throw();
            void build_tree() throw();
            void GraphViz(const string &filename) const;

            //! emit char, update model
            void encode(iobits &io, const char C);
            void encode_eos(iobits &io) const; //!< emit eos and pad to byte

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
            size_t wlen;
            void  *wksp;
            bool split( Node *source, const Char *head, const Char *tail, const size_t size, size_t &inode) throw();
            Node *getNode( size_t &inode );
        };
    };
}

#endif

