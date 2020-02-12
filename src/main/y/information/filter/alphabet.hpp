
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

            typedef size_t        FreqType;      //!< alias
            typedef size_t        CodeType;      //!< alias
            static const size_t   Chars = 256;   //!< encoding bytes
            static const CodeType NYT   = Chars; //!< Not Yet Transmitter
            static const CodeType EOS   = NYT+1; //!< End Of Stream
            static const size_t   Codes = EOS+1; //!< maximum possible codes
            static const CodeType Built = Codes; //!< built marker

            static const char *NameOf( const CodeType byte ) throw();

            //! building
            class Node : public ios::vizible
            {
            public:
                const CodeType symbol;    //!< encoding
                FreqType       frequency; //!< frequency
                CodeType       code;      //!< encoded char
                union
                {
                    Node *next;
                    Node *right;
                };
                union
                {
                    Node *prev;
                    Node *left;
                };
                size_t   bits; //!< current bits

                explicit Node(const CodeType leaf) throw(); //!< build a leaf
                explicit Node() throw();                    //!< build extra

                virtual void vizCore(ios::ostream &) const;

                class FrequencyComparator
                {
                public:
                    FrequencyComparator();
                    ~FrequencyComparator();
                    int operator()(const Node&,const Node&) throw();
                };

            private:
                virtual ~Node() throw();
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

            typedef core::list_of<Node> List;
            
            explicit Alphabet(const size_t numNodes,
                              const Mode   operating,
                              const size_t extraBytes);
            
            virtual ~Alphabet() throw();

            const Mode   mode;
            
        protected:
            List         alpha;
            const size_t count;
            const size_t shift;
            size_t       bytes;
            Node        *nodes;
            void        *extra;
            void         format() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);

        };


    }
}

#endif

