
//! \file
#ifndef Y_INFORMATION_ALPHABET_INCLUDED
#define Y_INFORMATION_ALPHANET_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/sort/merge.hpp"

namespace upsylon {

    namespace information {

        class Alphabet
        {
        public:
            //! operating on buffer or streams
            enum Mode
            {
                StreamMode, //!< will use EOS
                BufferMode  //!< will not use EOS
            };

            typedef size_t        FreqType;          //!< alias
            typedef size_t        CodeType;          //!< alias
            static const size_t   Chars = 256;       //!< encoding bytes
            static const CodeType NYT   = Chars;     //!< Not Yet Transmitter
            static const CodeType EOS   = NYT+1;     //!< End Of Stream
            static const size_t   Codes = EOS+1;     //!< maximum possible codes
            static const CodeType Built = Codes;     //!< built marker
            static const size_t   Alive = Chars+1;   //!< never more in alphabet
            static const size_t   Nodes = 2*Alive+1; //!< maximum number of nodes

            static const char *NameOf( const CodeType byte ) throw();

            //! building
            class Node : public ios::vizible
            {
            public:
                const CodeType symbol;    //!< encoding
                FreqType       frequency; //!< frequency
                CodeType       code;      //!< encoded char
                size_t         bits;      //!< current bits
                Node          *next;
                Node          *prev;
                Node          *left;
                Node          *right;


                explicit Node(const CodeType _code, const size_t _bits) throw(); //!< build a leaf
                explicit Node() throw();                                         //!< build internal

                virtual void vizCore(ios::ostream &) const;
                void    emit( qbits &io ) const;

                class FrequencyComparator
                {
                public:
                    FrequencyComparator() throw();
                    ~FrequencyComparator() throw();
                    int operator()(const Node&,const Node&) throw();
                };

                void display() const;

            private:
                virtual ~Node() throw();
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

            typedef core::list_of<Node> List;
            
            explicit Alphabet(const Mode   operating,
                              const size_t extraBytes);
            
            virtual ~Alphabet() throw();

            Node        *emit(qbits &io, const uint8_t u);

            const Mode   mode;  //!< operating mode
            const size_t level; //!< number of different emitted chars
            const size_t shift; //!< memory shift to get extra
            const size_t bytes; //!< allocated bytes

            void displayAlpha() const;

        protected:
            List         alpha; //!< currently used alphabet
            Node        *nodes; //!< linear access to nodes
            Node        *nyt;   //!< NYT node
            Node        *eos;   //!< EOS node
            
            void         initialize() throw();
            void        *extra()      throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);

        };


    }
}

#endif

