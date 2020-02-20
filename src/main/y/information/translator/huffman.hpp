//! \file

#ifndef Y_TRANSLATOR_HUFFMAN_INCLUDED
#define Y_TRANSLATOR_HUFFMAN_INCLUDED 1

#include "y/information/translator/alphabet.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/ordered/priority-queue.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            //! build an adaptice Huffman tree
            class Tree : public Alphabet
            {
            public:
                //--------------------------------------------------------------
                //
                // types and definitions
                //
                //--------------------------------------------------------------
                static const size_t Nodes  = 2*Alive+1; //!< maximum number of nodes
                static const size_t Bits   = 16;        //!< maxium code bits

                //! a node of the tree
                class Node : public ios::vizible
                {
                public:
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

                //--------------------------------------------------------------
                //
                // public methods
                //
                //--------------------------------------------------------------
                virtual     ~Tree() throw();            //!< cleanup
                const Node & getRoot() const throw();   //!< get root node

            protected:
                Node    *root; //!< root of the tree
                
                explicit Tree();                                //!< initialize
                void     update(const uint8_t byte, qbits *io); //!< emit current code if io!=NULL, and updateByte
                void     restart() throw();                     //!< initialize alphabet and build
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Tree);
                size_t       treeBytes;
                size_t       fullBytes;
                void        *workspace;
                Node        *treeNodes;
                Node::PQueue pq;
                
                void build() throw();
            };

        }

    }
}

#endif

