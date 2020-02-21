//! \file

#ifndef Y_TRANSLATOR_SHANNON_FANO_INCLUDED
#define Y_TRANSLATOR_SHANNON_FANO_INCLUDED 1

#include "y/information/translator/tree.hpp"
#include "y/ios/tools/vizible.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            //! a node of the ShannonFano tree
            class Node : public ios::vizible
            {
            public:
                typedef Alphabet::Char     Char;     //!< alias
                typedef Alphabet::CodeType CodeType; //!< alias

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


            //! build an adaptive Shannon-Fano tree
            class Tree : public TreeOf<Node>
            {
            public:
                virtual     ~Tree() throw();          //!< cleanup
                
            protected:
                explicit Tree(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Tree);
                virtual void buildTree() throw();
                bool split(Node *, size_t &) throw();
            };
        }
    }
}

#endif

