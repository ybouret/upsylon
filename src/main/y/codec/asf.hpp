//! \file
#ifndef Y_CODEC_ASF_INCLUDED
#define Y_CODEC_ASF_INCLUDED 1

#include "y/codec/iobits.hpp"
#include "y/ios/codec.hpp"

#include <iostream>
namespace upsylon
{
    //! adaptive Shannon-Fano
    struct ASF
    {
        typedef      size_t    CodeType;  //!< alias
        typedef      size_t    FreqType;  //!< alias
        static const size_t    NUM_CHARS     = 0x100;           //!< symbols an alphabet
        static const CodeType  NYT           = NUM_CHARS;      //!< Not Yet Transmitted
        static const CodeType  EOS           = NYT+1;          //!< End Of Stream
        static const size_t    ALPHABET_SIZE = NUM_CHARS+2; //!< chars+controls
        struct Char; //!< forward declaration

        //! for building the tree
        struct Node
        {
            Node       *left;  //!< for tree
            Node       *right; //!< for tree
            FreqType    freq;  //!< total frequency
            const Char *ch;    //!< terminal node
            CodeType    code;  //!< prefix code
            FreqType    bits;  //!< prefix bits
            void        viz( ios::ostream & ) const; //!< for GraphViz, recursive
        };

        //! alphabet node
        struct Char
        {
            Char             *next; //!< for list
            Char             *prev; //!< for list
            CodeType          byte; //!< real byte
            FreqType          freq; //!< its frequency
            mutable CodeType  code; //!< current code
            mutable FreqType  bits; //!< current bits
            mutable FreqType  part; //!< to compute splitting
            typedef core::list_of<Char> List; //!< alias
            const char *text() const throw(); //!< printable representation
        };

        //! operations on alphabet
        class Alphabet
        {
        public:
            Alphabet(); //!< initialize
            ~Alphabet() throw(); //!< destructor

            void       reset() throw(); //!< setup all
            Char::List active; //!< chars in use
            size_t     used;   //!< used char, bookeeping 0..256
            Char      *chars;  //!< memory for chars
            Node      *nodes;  //!< memory for nodes
            Char      *nyt;    //!< Not Yet Transmitterd char
            Char      *eos;    //!< End of Stream char

            //! display current encodig
            void display(std::ostream &os) const;

            //  generic add char
            // void add(const char C) throw();

            //! build the tree
            void build_tree() throw();

            //! save/render the tree
            void GraphViz(const string &filename) const;

            //! emit char, update model
            void encode(iobits &io, const char C);

            //! emit eos and pad to byte
            void encode_eos(iobits &io) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
            size_t wlen;
            void  *wksp;
            bool split( Node *source, const Char *head, const Char *tail, const size_t size, size_t &inode) throw();
            Node *getNode( size_t &inode );
        };

        //! encoder
        class Encoder : public ios::q_codec
        {
        public:
            explicit Encoder();         //!< initialize
            virtual ~Encoder() throw(); //!< destructor

            virtual void reset() throw(); //!< reset/free all
            virtual void flush();         //!< emit EOS
            virtual void write( char C ); //!< emit/update tree

        private:
            iobits   io;
            Alphabet alpha;
            Y_DISABLE_COPY_AND_ASSIGN(Encoder);
        };

        //! decoder
        class Decoder : public ios::q_codec
        {
        public:
            explicit Decoder();
            virtual ~Decoder() throw();

            virtual void reset() throw(); //!< reset/free all
            virtual void flush();         //!< emit EOS
            virtual void write( char C ); //!< emit/update tree

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Decoder);
            enum Status
            {
                wait_for_byte
            };
            Status      status;
            const Node *current;
            iobits      io;
            Alphabet    alpha;
            void        process();
        };


    };
}

#endif

