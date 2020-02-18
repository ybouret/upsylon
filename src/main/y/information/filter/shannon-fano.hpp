
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
                Node  *root; //!< root node of the enc/decoding tree
                void   update( Node *node ) throw();  //!< put node into position
                void   buildTree() throw();           //!< build the tree
                void   setupTree() throw();           //!< initialize alphabet and build tree

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);
                bool split(Node *parent, const size_t length, size_t &iNode) throw();
                
                Node *MakeL(Node *prev,
                            Node *node,
                            const size_t size,
                            size_t      &iNode) throw();
                
                static Node *Divide(Node   *hi,
                                    size_t &nhi,
                                    Node   *lo,
                                    size_t &nlo,
                                    const size_t n) throw();
               
            };


        }



    }


}

#endif
