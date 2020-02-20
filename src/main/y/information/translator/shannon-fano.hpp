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

                //! a node of the tree
                class Node : public ios::vizible
                {
                public:
                    //! setup with chars range
                    explicit Node(const Char *h, const Char *l) throw();

                    const Char *heavy; //!< heavy char
                    const Char *light; //!< light char

                    Node       *left;   //!< for tree
                    Node       *right;  //!< for tree
                    CodeType    code;   //!< current code
                    size_t      bits;   //!< current bits

                    //! vizible interface
                    virtual void vizCore( ios::ostream & ) const;
                    
                private:
                    virtual ~Node() throw();
                    Y_DISABLE_COPY_AND_ASSIGN(Node);
                };

                virtual     ~Tree() throw(); //!< cleanup
                const Node & getRoot() const throw();

            protected:
                explicit Tree(); //!< setup
                void update(const uint8_t byte, qbits *io); //!< emit and build
                void restart() throw();
                
                Node *root; //!< root
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Tree);
                size_t treeBytes;
                Node  *treeNodes;

                void build() throw();
                bool split(Node *node, size_t &inode) throw();
            };
        }
    }
}

#endif

