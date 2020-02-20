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
                    explicit Node() throw();
                    const Char *chr;

                    Node *left;
                    Node *right;


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Node);
                };

                explicit Tree();
                virtual ~Tree() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Tree);
                size_t treeBytes;
                Node  *treeNodes;
            };
        }
    }
}

#endif

