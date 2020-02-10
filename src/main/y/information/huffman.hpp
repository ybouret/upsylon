//! \file
#ifndef Y_INFORMATION_HUFFMAN_INCLUDED
#define Y_INFORMATION_HUFFMAN_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/ordered/heap.hpp"
#include "y/ios/tools/vizible.hpp"

namespace upsylon {

    namespace information {

        struct Huffman
        {
            enum Mode
            {
                StreamMode,
                BufferMode
            };

            static const size_t   NumChars = 256;
            typedef size_t        CodeType;
            typedef size_t        FreqType;
            static const CodeType NYT          = NumChars;
            static const CodeType EOS          = NYT+1;
            static const size_t   AlphabetSize = EOS+1;
            static const size_t   NumNodes     = (2*AlphabetSize)+1;

            class Node : public ios::vizible {
            public:
                CodeType code;
                CodeType bits;
                FreqType freq;
                Node    *next;
                Node    *prev;
                Node    *left;
                Node    *right;


                struct Comparator
                {
                    int operator()( const Node &lhs, const Node &rhs ) throw()
                    {
                        return comparison::decreasing(lhs.freq,rhs.freq);
                    }
                };

                virtual void vizCore( ios::ostream &fp ) const;
                explicit Node() throw();
                virtual ~Node() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);

            };

            typedef core::list_of<Node>          List;
            typedef heap<Node,Node::Comparator>  Heap;

            class Tree
            {
            public:
                Tree(const Mode mode) throw();
                ~Tree() throw();

                void reset(const Mode mode) throw();
                inline const Node &getRoot() const throw() { assert(root); return *root; }

                void encode( qbits &io, const uint8_t byte );

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Tree);
                Node        *root;
                List         alpha;
                Heap         pq;
                size_t       nodeCount;
                size_t       nodeBytes;
                Node        *nodes;
                Node        *nyt;
                Node        *eos;
                
                void build() throw();
                void clear() throw();
            };

        };

    }

    
}

#endif
