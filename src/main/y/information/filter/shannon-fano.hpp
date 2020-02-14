
//! \file
#ifndef Y_INFORMATION_SHANNON_FANO_INCLUDED
#define Y_INFORMATION_SHANNON_FANO_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/information/filter/alphabet.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {
            
            //! context based on alphabet, used to build tree
            class Context : public Alphabet
            {
            public:
                virtual ~Context() throw();   //!< cleanup
                explicit Context(const Mode); //!< setup
                
                const Node &getRoot() const throw(); //!< get the root node

            protected:
                Node  *root;
                void   update( Node *node ) throw();  //!< put node into position
                void   buildTree() throw();           //!< build the tree
                void   setupTree() throw();           //!< initialize alphabet and build tree

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);
                void  split(Node *curr, size_t &iNode) throw();

            };


        }



    }


}

#endif
