//! \file

#ifndef Y_TRANSLATOR_SHANNON_FANO_INCLUDED
#define Y_TRANSLATOR_SHANNON_FANO_INCLUDED 1

#include "y/information/translator/alphabet.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/ordered/priority-queue.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            //! build an adaptive Shannon-Fano tree
            class Tree : public Alphabet
            {
            public:
                //--------------------------------------------------------------
                //
                // types and definitions
                //
                //--------------------------------------------------------------
                static const size_t Nodes  = 2*Alive+1; //!< maximum number of nodes

                class Node
                {
                public:
                    explicit Node(const Char *h, const Char *l) throw();

                    const Char *heavy;
                    const Char *light;

                    Node       *left;   //!< for tree
                    Node       *right;  //!< for tree
                    CodeType    code;   //!< current code
                    size_t      bits;   //!< current bits

                private:
                    virtual ~Node() throw();
                    Y_DISABLE_COPY_AND_ASSIGN(Node);
                };

                explicit Tree();
                virtual ~Tree() throw();

                void update(const uint8_t byte)
                {
                    updateByte(byte, NULL);
                    build();
                }


            protected:
                Node *root;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Tree);
                size_t treeBytes;
                Node  *treeNodes;

                void build() throw();
                bool split(Node   *node,
                           size_t &inode) throw();
            };
        }
    }
}

#endif

