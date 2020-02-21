//! \file

#ifndef Y_TRANSLATOR_HUFFMAN_INCLUDED
#define Y_TRANSLATOR_HUFFMAN_INCLUDED 1

#include "y/information/translator/tree.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/ordered/priority-queue.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            //! a node of the Huffman tree
            class Node : public ios::vizible
            {
            public:
                typedef Alphabet::FreqType FreqType;
                typedef Alphabet::CodeType CodeType;
                typedef Alphabet::Char     Char;
                
                //! initialize
                explicit Node(const Char   * _char,
                              const FreqType _freq,
                              const size_t   _bits) throw();

                const Char *chr;    //!< leaf Char, NULL if internal
                Node       *left;   //!< at left
                Node       *right;  //!< at right
                FreqType    freq;   //!< local frequency
                CodeType    code;   //!< local code
                size_t      bits;   //!< local bits

                //! GraphViz operations
                virtual void vizCore(ios::ostream &) const;

                //! comparator for priority queue
                struct Comparator
                {
                    //! use frequency
                    int operator()(const Node &,const Node&) const throw();
                };

                //! priority queue operations
                typedef core::priority_queue<Node,Comparator> PQueue;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
                virtual ~Node() throw();
            };

            //! build an adaptice Huffman tree
            class Tree : public TreeOf<Node>
            {
            public:
                virtual ~Tree() throw();            //!< cleanup

            protected:
                
                explicit Tree();                                //!< initialize
                void     update(const uint8_t byte, qbits *io); //!< emit current code if io!=NULL, and updateByte
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Tree);
                virtual void buildTree() throw();

                Node::PQueue pq;

            };

        }

    }
}

#endif

