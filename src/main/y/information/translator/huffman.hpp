//! \file


#ifndef Y_TRANSLATOR_HUFFMAN_INCLUDED
#define Y_TRANSLATOR_HUFFMAN_INCLUDED 1

#include "y/information/translator.hpp"
#include "y/information/translator/alphabet.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/ordered/priority-queue.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            class Tree : public Alphabet
            {
            public:
                static const size_t Nodes = 2*Alive+1;

                class Node : public ios::vizible
                {
                public:
                    explicit Node(const Char   * _char,
                                  const FreqType _freq,
                                  const size_t   _bits) throw();
                    const Char *chr;
                    Node       *left;
                    Node       *right;
                    FreqType    freq;
                    CodeType    code;
                    size_t      bits;
                    virtual void vizCore(ios::ostream &) const;

                    struct Comparator
                    {
                        int operator()(const Node &,const Node&) const throw();
                    };

                    typedef core::priority_queue<Node,Comparator> PQueue;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Node);
                    virtual ~Node() throw();
                };



                explicit Tree();
                virtual ~Tree() throw();
                const Node & getRoot() const throw();

                void update(const uint8_t byte, qbits *io);

            protected:
                Node        *root;

                
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

